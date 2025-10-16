#!/usr/bin/env python3
"""
Replace hand-inlined lengthOf helpers with shared function calls.

The generator expands several variants:
  - lengthOf:
  - lengthOf:format:
  - lengthOf:baseHeader:format:

Each variant produces long chains of conditionals assigning to the same target
variable. This script contracts those blocks to a single helper invocation.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import Iterable, List, Tuple

BEGIN_PREFIX = "/* begin lengthOf:"
END_PREFIX = "/* end lengthOf:"

HELPER_FILES = {
    "lengthOf.c",
    "lengthOfformat.c",
    "lengthOfMaybeImmediate.c",
    "lengthOfNameOfClass.c",
}


def strip_outer_parens(expr: str) -> str:
    expr = expr.strip()
    while expr.startswith("(") and expr.endswith(")"):
        depth = 0
        balanced = True
        for index, ch in enumerate(expr):
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth < 0:
                    balanced = False
                    break
                if depth == 0 and index != len(expr) - 1:
                    balanced = False
                    break
        if not balanced or depth != 0:
            break
        expr = expr[1:-1].strip()
    return expr


def extract_object(block: List[str]) -> str | None:
    joined = "".join(block)
    match = re.search(r"numSlotsOfAny\(\s*([^)]+?)\s*\)", joined)
    if match:
        return strip_outer_parens(match.group(1))
    match = re.search(
        r"\(\s*([^)]+?)\s*\+\s*\(formatFieldByteOffset\(\)\)\s*\)", joined
    )
    if match:
        return strip_outer_parens(match.group(1))
    return None


def extract_format_var(block: List[str]) -> str | None:
    for line in block:
        match = re.search(r"if\s*\(\s*\(*\s*([A-Za-z_][A-Za-z0-9_]*)", line)
        if match:
            candidate = match.group(1)
            if candidate not in {"numSlots", "fmtSqInt"} or "fmtSqInt" in line:
                return candidate
    return None


def extract_result_var(block: List[str]) -> str | None:
    skip = {"numSlots", "fmt", "fmtSqInt"}
    assignment = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=")
    for line in block:
        match = assignment.match(line)
        if match:
            candidate = match.group(1)
            if candidate not in skip:
                return candidate
    return None


def rewrite_block(
    block: List[str], block_type: str, indent: str
) -> Tuple[List[str], bool]:
    obj = extract_object(block)
    if not obj:
        return block, False

    result_var = extract_result_var(block)
    if not result_var:
        return block, False

    if block_type == "plain":
        call_line = f"{indent}{result_var} = lengthOf({obj});\n"
    else:
        fmt_var = extract_format_var(block)
        if not fmt_var:
            return block, False
        call_line = f"{indent}{result_var} = lengthOfformat({obj}, {fmt_var});\n"

    return [call_line], True


def process_text(text: str) -> Tuple[str, bool]:
    lines = text.splitlines(keepends=True)
    output: List[str] = []
    i = 0
    changed = False

    while i < len(lines):
        line = lines[i]
        if BEGIN_PREFIX in line:
            block_lines = [line]
            block_type = "plain"
            if "baseHeader" in line:
                block_type = "base"
            elif "format" in line:
                block_type = "format"

            indent = line[: len(line) - len(line.lstrip())]
            j = i + 1
            while j < len(lines):
                block_lines.append(lines[j])
                if END_PREFIX in lines[j]:
                    break
                j += 1
            else:
                output.extend(block_lines)
                break

            rewritten_block, did_change = rewrite_block(
                block_lines, "plain" if block_type == "plain" else "format", indent
            )
            output.extend(rewritten_block)
            changed = changed or did_change
            i = j + 1
            continue

        output.append(line)
        i += 1

    return "".join(output), changed


def iter_sources(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            yield from (
                candidate
                for candidate in path.rglob("*.c")
                if candidate.name not in HELPER_FILES
            )
        elif path.suffix == ".c" and path.name not in HELPER_FILES:
            yield path


def main(argv: List[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline lengthOf blocks with helper calls."
    )
    parser.add_argument("paths", nargs="+", help="Files or directories to process.")
    args = parser.parse_args(argv)

    any_changed = False
    for source in iter_sources(Path(p) for p in args.paths):
        original = source.read_text()
        rewritten, changed = process_text(original)
        if changed:
            source.write_text(rewritten)
            print(f"Updated {source}", flush=True)
            any_changed = True

    return 0 if any_changed else 0


if __name__ == "__main__":
    import sys

    sys.exit(main(sys.argv[1:]))
