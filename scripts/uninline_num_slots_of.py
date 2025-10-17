#!/usr/bin/env python3
"""
Replace hand-inlined numSlotsOf: expansions with calls to numSlotsOf().

The generated interpreter sources often inline the logic for numSlotsOf:, either
as a large expression

    (((numSlots = byteAt(...))) == (numSlotsMask())
         ? ...
         : numSlots)

or as an if/else block that assigns to a destination variable.  This script
removes those inlined forms and rewrites them to the helper call
`numSlotsOf(objectExpr)`.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable

EXPR_PATTERN = re.compile(
    r"(?:\(\s*)+"
    r"(?P<tmp>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*"
    r"byteAt\(\s*(?:\(\s*)*\(\s*void\s*\*\)\s*\(\s*(?P<obj>.*?)\s*\+\s*"
    r"\(numSlotsFieldByteOffset\(\)\)\s*\)(?:\)\s*)+\)\s*\)\s*==\s*"
    r"\(numSlotsMask\(\)\)\s*\?(?P<true>.*?)\:\s*(?P<false>.*?)\)"
    r"(?=\s*[<),;])",
    re.DOTALL,
)
COMMENT_MARKER = "/* begin numSlotsOf: */"
ASSERT_LINE_RE = re.compile(
    r"^[ \t]*assert\(\(classIndexOf\([^)]+\)\) > "
    r"\(isForwardedObjectClassIndexPun\(\)\)\)[,;]\s*$"
)


def strip_prefix(prefix: str) -> str:
    """Remove trailing comment and assert lines that precede an inline expression."""

    comment_idx = prefix.rfind(COMMENT_MARKER)
    if comment_idx != -1:
        prefix = prefix[:comment_idx] + prefix[comment_idx + len(COMMENT_MARKER) :]

    lines = prefix.splitlines(keepends=True)
    trailing: list[str] = []
    while lines and lines[-1].strip() == "":
        trailing.insert(0, lines.pop())

    for idx in range(len(lines) - 1, -1, -1):
        if ASSERT_LINE_RE.match(lines[idx].rstrip("\n")):
            lines.pop(idx)
            break

    filtered_trailing = [text for text in trailing if not text.endswith("\n")]
    return "".join(lines + filtered_trailing)


def replace_inline_expressions(text: str) -> tuple[str, bool]:
    idx = 0
    pieces: list[str] = []
    changed = False

    for match in EXPR_PATTERN.finditer(text):
        raw_prefix = text[idx : match.start()]
        cleaned_prefix = strip_prefix(raw_prefix)
        obj = match.group("obj").strip()
        true_branch = match.group("true").strip()
        false_branch = match.group("false").strip()

        dest = None
        assign_re = re.compile(r"\(?\s*([A-Za-z_][A-Za-z0-9_]*)\s*=")
        true_match = assign_re.match(true_branch)
        false_match = assign_re.match(false_branch)
        if true_match and false_match and true_match.group(1) == false_match.group(1):
            dest = true_match.group(1)

        pieces.append(cleaned_prefix)
        if dest:
            leading_len = match.start("tmp") - match.start()
            leading_segment = match.group(0)[:leading_len]
            trailing_segment = text[match.end("false") : match.end()]
            original_text = match.group(0)

            balance_original = original_text.count("(") - original_text.count(")")
            balance_repl = (
                leading_segment.count("(")
                - leading_segment.count(")")
                + trailing_segment.count("(")
                - trailing_segment.count(")")
            )
            adjustment = balance_repl - balance_original
            if adjustment > 0:
                trailing_segment += ")" * adjustment
            elif adjustment < 0:
                leading_segment = "(" * (-adjustment) + leading_segment

            pieces.append(
                f"{leading_segment}{dest} = numSlotsOf({obj}){trailing_segment}"
            )
        else:
            pieces.append(f"numSlotsOf({obj})")
        idx = match.end()
        changed = True

    pieces.append(text[idx:])
    return "".join(pieces), changed


def replace_if_blocks(text: str) -> tuple[str, bool]:
    lines = text.splitlines(keepends=True)
    output: list[str] = []
    i = 0
    changed = False

    while i < len(lines):
        line = lines[i]
        if line.strip() == COMMENT_MARKER and i + 1 < len(lines):
            assert_line = lines[i + 1]
            assert_match = re.match(
                r"\s*assert\(\(classIndexOf\((?P<obj>[^)]+)\)\) > "
                r"\(isForwardedObjectClassIndexPun\(\)\)\);\s*",
                assert_line,
            )
            if assert_match:
                obj = assert_match.group("obj").strip()
                j = i + 2
                brace = 0
                seen_open = False
                block_lines: list[str] = []
                while j < len(lines):
                    block_lines.append(lines[j])
                    if "{" in lines[j]:
                        seen_open = True
                    brace += lines[j].count("{")
                    brace -= lines[j].count("}")
                    if seen_open and brace <= 0:
                        break
                    j += 1
                block_text = "".join(block_lines)
                if "byteAt" in block_text and "longAt" in block_text and "else" in block_text:
                    if_line = block_lines[0]
                    if_match = re.search(
                        r"if \(\(\((?P<tmp>[A-Za-z_][A-Za-z0-9_]*)\s*=",
                        if_line,
                    )
                    dest = None
                    for block_line in block_lines[1:]:
                        dest_match = re.match(r"\s*([A-Za-z_][A-Za-z0-9_]*)\s*=", block_line)
                        if dest_match:
                            dest = dest_match.group(1)
                            break
                    if if_match and dest:
                        indent = line[: len(line) - len(line.lstrip(" \t"))]
                        output.append(f"{indent}{dest} = numSlotsOf({obj});\n")
                        i = j + 1
                        changed = True
                        continue
        output.append(line)
        i += 1

    return "".join(output), changed


def process_text(text: str) -> tuple[str, bool]:
    after_blocks, blocks_changed = replace_if_blocks(text)
    after_exprs, exprs_changed = replace_inline_expressions(after_blocks)
    return after_exprs, blocks_changed or exprs_changed


def iter_source_files(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            yield from (p for p in path.rglob("*.c") if p.name != "numSlotsOf.c")
        elif path.suffix == ".c" and path.name != "numSlotsOf.c":
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline numSlotsOf expansions to numSlotsOf() calls."
    )
    parser.add_argument("paths", nargs="+", help="Source files or directories to process.")
    args = parser.parse_args(argv)

    any_changed = False
    paths = [Path(p) for p in args.paths]

    for source in iter_source_files(paths):
        original = source.read_text()
        new_text, changed = process_text(original)
        if changed:
            source.write_text(new_text)
            print(f"Updated {source}", file=sys.stderr)
            any_changed = True

    return 0 if any_changed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
