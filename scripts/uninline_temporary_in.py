#!/usr/bin/env python3
"""
Replace hand-inlined temporary:in: expansions with calls to temporaryin().

The translated Spur interpreter sources often inline the temporary lookup as:

    /* temporary:in: */
    (offset < ((frameNumArgs = byteAt((theFP + FoxFrameFlags) + 1)))
         ? longAt((theFP + FoxCallerSavedIP) +
                  ((frameNumArgs - offset) * BytesPerWord))
         : longAt(((theFP + FoxReceiver) - BytesPerWord) +
                  ((frameNumArgs - offset) * BytesPerWord)))

This script collapses the block to `temporaryin(offset, theFP)`, preserving the
original offset expression and frame pointer identifier.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable, Iterator, Optional, Tuple

COMMENT = "/* temporary:in: */"

COND_RE = re.compile(
    r"""
    ^\s*(?P<offset>.+?)\s*<\s*
    \(\(frameNumArgs\s*=\s*byteAt\(\(\s*
    (?P<fp>.+?)
    \s*\+\s*FoxFrameFlags\)\s*\+\s*1\)\)\)\s*$
    """,
    re.DOTALL | re.VERBOSE,
)


def strip_enclosing_parens(expr: str) -> str:
    expr = expr.strip()
    while expr.startswith("(") and expr.endswith(")"):
        inner = expr[1:-1].strip()
        if not inner:
            break
        depth = 0
        balanced = True
        for idx, ch in enumerate(inner):
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth < 0:
                    balanced = False
                    break
            if depth == 0 and idx != len(inner) - 1:
                balanced = False
                break
        if not balanced or depth != 0:
            break
        expr = inner
    return expr


def split_ternary(expr: str) -> Optional[Tuple[str, str, str]]:
    depth = 0
    q_index = None
    colon_index = None
    for i, ch in enumerate(expr):
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        elif ch == "?" and depth == 0 and q_index is None:
            q_index = i
        elif ch == ":" and depth == 0 and q_index is not None:
            colon_index = i
            break
    if q_index is None or colon_index is None:
        return None
    condition = expr[:q_index].strip()
    true_branch = expr[q_index + 1 : colon_index].strip()
    false_branch = expr[colon_index + 1 :].strip()
    return condition, true_branch, false_branch


def parse_condition(expr: str) -> Optional[Tuple[str, str]]:
    cleaned = strip_enclosing_parens(expr)
    match = COND_RE.match(cleaned)
    if not match:
        return None
    offset = strip_enclosing_parens(match.group("offset"))
    fp = strip_enclosing_parens(match.group("fp"))
    return offset.strip(), fp.strip()


def find_matching_paren(text: str, start: int) -> Optional[int]:
    depth = 0
    i = start
    # Simple parenthesis matcher; expressions do not contain string literals.
    while i < len(text):
        ch = text[i]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                return i
            if depth < 0:
                return None
        i += 1
    return None


def build_replacement(expr: str) -> Optional[str]:
    expr = expr.strip()
    if not expr.startswith("(") or not expr.endswith(")"):
        return None
    inner = expr[1:-1].strip()
    ternary = split_ternary(inner)
    if not ternary:
        return None
    condition, _true_branch, _false_branch = ternary
    parsed = parse_condition(condition)
    if not parsed:
        return None
    offset, fp = parsed
    return f"temporaryin({offset}, {fp})"


def process_text(text: str) -> Tuple[str, bool]:
    result_parts: list[str] = []
    idx = 0
    changed = False

    while True:
        pos = text.find(COMMENT, idx)
        if pos == -1:
            result_parts.append(text[idx:])
            break

        result_parts.append(text[idx:pos])
        after = pos + len(COMMENT)

        expr_start = after
        while expr_start < len(text) and text[expr_start].isspace():
            expr_start += 1

        if expr_start >= len(text) or text[expr_start] != "(":
            # Unexpected structure; keep comment as-is.
            result_parts.append(text[pos:after])
            idx = after
            continue

        expr_end = find_matching_paren(text, expr_start)
        if expr_end is None:
            # Unbalanced parentheses; leave original text untouched.
            result_parts.append(text[pos:])
            idx = len(text)
            break

        expr = text[expr_start : expr_end + 1]
        replacement = build_replacement(expr)
        if not replacement:
            result_parts.append(text[pos : expr_end + 1])
        else:
            result_parts.append(replacement)
            changed = True
        idx = expr_end + 1

    return "".join(result_parts), changed


def iter_source_files(paths: Iterable[Path]) -> Iterator[Path]:
    for path in paths:
        if path.is_dir():
            yield from (
                p
                for p in path.rglob("*.c")
                if p.name != "temporaryin.c"
            )
        elif path.suffix == ".c" and path.name != "temporaryin.c":
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline temporary:in: expansions."
    )
    parser.add_argument(
        "paths", nargs="+", help="Files or directories to process."
    )
    args = parser.parse_args(argv)

    any_changes = False
    for source in iter_source_files(Path(p) for p in args.paths):
        original = source.read_text()
        updated, changed = process_text(original)
        if changed:
            source.write_text(updated)
            print(f"Updated {source}", file=sys.stderr)
            any_changes = True
    return 0 if any_changes else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
