#!/usr/bin/env python3
"""
Replace hand-inlined pop:thenPush: blocks with popthenPush helper calls.

The generated interpreter sources expand the helper as:

    /* begin pop:thenPush: */
    longAtput((sp = stackPointer + (<expr> * BytesPerWord)), value);
    stackPointer = sp;

This script rewrites those sequences to the compact helper form:

    /* begin pop:thenPush: */
    popthenPush((<expr> + 1), value);
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import Iterable

COMMENT = "/* begin pop:thenPush: */"
STACK_POINTER_LINE = re.compile(r"^\s*stackPointer\s*=\s*sp;\s*$")
MULTIPLIER_RE = re.compile(
    r"stackPointer\s*\+\s*\(\s*(?P<mul>.+?)\s*\*\s*BytesPerWord\s*\)"
)


def strip_outer_parens(expr: str) -> str:
    expr = expr.strip()
    while expr.startswith("(") and expr.endswith(")"):
        depth = 0
        balanced = True
        for index, char in enumerate(expr):
            if char == "(":
                depth += 1
            elif char == ")":
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


def find_top_level_operator(expr: str, operator: str) -> int | None:
    depth = 0
    for index, char in enumerate(expr):
        if char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
        elif char == operator and depth == 0:
            return index
    return None


def finalize_items_expr(multiplier: str) -> str:
    stripped = strip_outer_parens(multiplier)
    if not stripped:
        return "1"
    if re.fullmatch(r"[+-]?\d+", stripped):
        return str(int(stripped) + 1)

    minus_index = find_top_level_operator(stripped, "-")
    if minus_index is not None:
        left = stripped[:minus_index].strip()
        right = stripped[minus_index + 1 :].strip()
        right_clean = strip_outer_parens(right)
        if right_clean in {"1", "1U", "1u"}:
            left_clean = strip_outer_parens(left)
            return left_clean if left_clean else "1"

    has_top_plus = find_top_level_operator(stripped, "+") is not None
    has_top_minus = find_top_level_operator(stripped, "-") is not None
    if not has_top_plus and not has_top_minus:
        return f"{stripped} + 1"

    return f"({stripped}) + 1"


def simplify_argument_expression(argument: str) -> tuple[str, bool]:
    stripped = argument.strip()
    if not stripped:
        return argument, False

    if re.fullmatch(r"[+-]?\d+", strip_outer_parens(stripped)):
        normalized = strip_outer_parens(stripped)
        return normalized, normalized != argument

    plus_index = find_top_level_operator(stripped, "+")
    if plus_index is not None:
        left = stripped[:plus_index].strip()
        right = stripped[plus_index + 1 :].strip()
        right_clean = strip_outer_parens(right)
        if right_clean in {"1", "1U", "1u"}:
            replacement = finalize_items_expr(left)
            return replacement, replacement != argument

    return argument, False


def extract_multiplier(line: str) -> str | None:
    match = MULTIPLIER_RE.search(line)
    if match:
        return match.group("mul").strip()
    return None


def extract_value(lines: list[str]) -> str | None:
    if not lines:
        return None
    combined = "".join(lines)
    if "," not in combined:
        return None
    after_comma = combined.split(",", 1)[1]
    if ");" not in after_comma:
        return None
    value_text = after_comma.rsplit(");", 1)[0]
    return value_text.strip()


def process_text(text: str) -> tuple[str, bool]:
    lines = text.splitlines(keepends=True)
    output: list[str] = []
    i = 0
    changed = False

    while i < len(lines):
        line = lines[i]
        if COMMENT in line and i + 2 < len(lines):
            j = i + 1
            statement_lines = [lines[j]]
            while j + 1 < len(lines) and ");" not in statement_lines[-1]:
                j += 1
                statement_lines.append(lines[j])
            statement_text = "".join(statement_lines)
            multiplier = extract_multiplier(statement_text)
            value = extract_value(statement_lines)
            stack_line_index = j + 1
            if (
                multiplier
                and value
                and stack_line_index < len(lines)
                and STACK_POINTER_LINE.match(lines[stack_line_index].strip("\n"))
            ):
                first_line = lines[i + 1]
                indent = first_line[: len(first_line) - len(first_line.lstrip())]
                items_expr = finalize_items_expr(multiplier)
                replacement = f"{indent}popthenPush({items_expr}, {value});\n"
                output.append(line)
                output.append(replacement)
                i = stack_line_index + 1
                changed = True
                continue
        output.append(line)
        i += 1

    rewritten = "".join(output)
    simplified, simplified_changed = simplify_existing_calls(rewritten)
    return simplified, changed or simplified_changed


def simplify_existing_calls(text: str) -> tuple[str, bool]:
    token = "popthenPush("
    result: list[str] = []
    index = 0
    changed = False

    while True:
        pos = text.find(token, index)
        if pos == -1:
            result.append(text[index:])
            break

        result.append(text[index : pos + len(token)])
        cursor = pos + len(token)
        depth = 0
        while cursor < len(text):
            ch = text[cursor]
            if ch == "(":
                depth += 1
            elif ch == ")":
                if depth > 0:
                    depth -= 1
            elif ch == "," and depth == 0:
                break
            cursor += 1
        if cursor >= len(text):
            result.append(text[pos + len(token) :])
            index = len(text)
            break

        original_arg = text[pos + len(token) : cursor]
        simplified_arg, did_change = simplify_argument_expression(original_arg)
        if did_change:
            changed = True
        result.append(simplified_arg)
        result.append(text[cursor])
        index = cursor + 1

    return "".join(result), changed


def iter_sources(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            yield from (
                candidate
                for candidate in path.rglob("*.c")
                if candidate.name != "popthenPush.c"
            )
        elif path.suffix == ".c" and path.name != "popthenPush.c":
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline pop:thenPush: blocks with popthenPush calls."
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
