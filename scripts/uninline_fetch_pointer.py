#!/usr/bin/env python3
"""
Replace inline expansions of fetchPointer:ofObject: with calls to
fetchPointerofObject(fieldIndex, object).

The generated C code frequently inlines expressions like:

    longAt((void *)((obj + BaseHeaderSize) +
                     ((((usqInt)(field) << (shiftForWord()))))));

This script locates such patterns (including multi-line variants with
excess parentheses) and rewrites them to the shorter helper form,
`fetchPointerofObject(field, obj)`.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Optional, Tuple

LONGAT_PREFIX = "longAt("


def strip_outer_parens_once(expr: str) -> str:
    expr = expr.strip()
    if not expr or expr[0] != "(" or expr[-1] != ")":
        return expr
    depth = 0
    for idx, ch in enumerate(expr):
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                if idx == len(expr) - 1:
                    return expr[1:-1].strip()
                return expr
    return expr


def strip_outer_parens(expr: str) -> str:
    previous = expr
    while True:
        current = strip_outer_parens_once(previous)
        if current == previous:
            return current
        previous = current


def find_token(expr: str, token: str) -> int:
    depth = 0
    idx = 0
    length = len(expr)
    while idx < length:
        ch = expr[idx]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth = max(depth - 1, 0)
        elif expr.startswith(token, idx) and depth == 0:
            return idx
        idx += 1
    return -1


def split_at(expr: str, token: str) -> Optional[Tuple[str, str]]:
    idx = find_token(expr, token)
    if idx == -1:
        return None
    return expr[:idx].strip(), expr[idx + len(token) :].strip()


def extract_field_expr(offset_expr: str) -> Optional[str]:
    comment_expr = None
    comment_match = re.search(r"/\*(.+?)\*/", offset_expr)
    if comment_match:
        comment_expr = comment_match.group(1).strip()
        offset_expr = offset_expr[: comment_match.start()].strip()

    expr = strip_outer_parens(offset_expr)
    expr = strip_outer_parens(expr)
    expr = strip_outer_parens(expr)
    expr = expr.strip()

    parts = split_at(expr, "<<")
    if not parts:
        # Fall back to extracting from the comment, if present.
        if comment_expr:
            comment_expr = comment_expr.replace("shiftForWord()", "shiftForWord")
            comment_expr = strip_outer_parens(comment_expr)
            comment_parts = split_at(comment_expr, "<<")
            if comment_parts and "shiftForWord" in comment_parts[1]:
                candidate = strip_outer_parens(comment_parts[0]).strip()
                return candidate if candidate else None
        return None
    left, right = parts
    if strip_outer_parens(right).strip() != "shiftForWord()":
        return None

    left = strip_outer_parens(left).strip()
    # Remove optional cast
    if left.startswith("(usqInt)"):
        left = left[len("(usqInt)") :].strip()
    elif left.startswith("((usqInt)"):
        left = left[len("((usqInt)") :].strip()
        if left.endswith(")"):
            left = left[:-1].strip()
    left = strip_outer_parens(left).strip()
    return left if left else None


def extract_object_and_field(body: str) -> Optional[Tuple[str, str]]:
    content = strip_outer_parens(body)
    parts = split_at(content, "+")
    if not parts:
        return None
    left, right = parts

    field_expr = extract_field_expr(right)
    if field_expr is None:
        return None

    left_expr = strip_outer_parens(left)
    object_parts = split_at(left_expr, "+")
    if not object_parts:
        return None
    object_expr, base_expr = object_parts
    if strip_outer_parens(base_expr).strip() != "BaseHeaderSize":
        return None
    object_expr = strip_outer_parens(object_expr).strip()
    if not object_expr:
        return None

    return object_expr, field_expr


def parse_longat_call(text: str, start: int) -> Optional[Tuple[int, str]]:
    call_open = text.find("(", start)
    if call_open == -1:
        return None

    depth = 0
    call_end = -1
    for idx in range(call_open, len(text)):
        ch = text[idx]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                call_end = idx + 1
                break
    if call_end == -1:
        return None

    call_text = text[call_open:call_end]
    call_stripped = call_text.strip()
    match = re.match(r"^\(\s*\(void\s*\*\)\s*(?P<body>.+)\)$", call_stripped, re.DOTALL)
    if not match:
        return None

    body = match.group("body").strip()
    parsed = extract_object_and_field(body)
    if not parsed:
        return None
    object_expr, field_expr = parsed
    replacement = f"fetchPointerofObject({field_expr}, {object_expr})"
    return call_end, replacement


def transform_text(text: str) -> Tuple[str, bool]:
    result = []
    idx = 0
    changed = False
    length = len(text)
    while idx < length:
        next_idx = text.find(LONGAT_PREFIX, idx)
        if next_idx == -1:
            result.append(text[idx:])
            break
        result.append(text[idx:next_idx])
        parsed = parse_longat_call(text, next_idx)
        if not parsed:
            # Leave the original text untouched if parsing failed.
            result.append(LONGAT_PREFIX)
            idx = next_idx + len(LONGAT_PREFIX)
            continue
        call_end, replacement = parsed
        result.append(replacement)
        idx = call_end
        changed = True
    return "".join(result), changed


def process_file(path: Path) -> bool:
    original = path.read_text()
    text = original
    changed_any = False
    while True:
        transformed, changed = transform_text(text)
        if not changed:
            break
        text = transformed
        changed_any = True
    # Remove remaining inline markers for fetchPointer.
    lines = []
    removed = False
    for line in text.splitlines(keepends=True):
        if "/* begin fetchPointer:ofObject: */" in line:
            removed = True
            continue
        lines.append(line)
    if removed:
        text = "".join(lines)
        changed_any = True
    if changed_any:
        path.write_text(text)
        print(f"Updated {path}", file=sys.stderr)
    return changed_any


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Replace inline fetchPointer expansions with helper calls."
    )
    parser.add_argument("files", nargs="+", help="C source files to rewrite.")
    args = parser.parse_args(argv)

    for name in args.files:
        path = Path(name)
        if not path.is_file():
            continue
        if path.name == "fetchPointerofObject.c":
            continue
        process_file(path)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
