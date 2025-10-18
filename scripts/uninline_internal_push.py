#!/usr/bin/env python3
"""
Replace inline internalPush expansions with calls to the shared helper.

The generated interpreter sources often inline stack pushes as:

    /* begin internalPush: */
    longAtput((localSP -= BytesPerOop), value);

or, when the value expression spans multiple lines:

    longAtput(
        (localSP -= BytesPerOop),
        someExpression);

This script rewrites both forms to:

    internalPush(&localSP, someExpression);

preserving indentation and multiline expressions.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable, Iterator, Tuple

PATTERN = re.compile(
    r"""
    (?P<indent>^[ \t]*)
    (?:/\*\s*begin\s+internalPush:\s*\*/\s*\r?\n(?P=indent))?
    longAtput
    \(
    (?P<ws1>\s*)
    \(
    \s*localSP\s*-\=\s*BytesPerOop\s*
    \)
    \s*,\s*
    (?P<expr>.*?)
    \)
    ;
    (?P<tail>[ \t]*\r?\n?)
    """,
    re.MULTILINE | re.DOTALL | re.VERBOSE,
)


def format_replacement(indent: str, expr: str, tail: str) -> str:
    expr = expr.strip()
    cont_indent = indent + " " * len("internalPush(&localSP, ")

    if "\n" not in expr:
        return f"{indent}internalPush(&localSP, {expr});{tail}"

    lines = [line.strip() for line in expr.splitlines()]
    formatted_lines = [f"{indent}internalPush(&localSP, {lines[0]}"]
    for line in lines[1:]:
        formatted_lines.append(f"{cont_indent}{line}")
    formatted_lines[-1] += ");"
    return "\n".join(formatted_lines) + tail


def replace_block(match: re.Match[str]) -> str:
    indent = match.group("indent")
    expr = match.group("expr")
    tail = match.group("tail") or "\n"
    return format_replacement(indent, expr, tail)


def process_text(text: str) -> Tuple[str, bool]:
    new_text, count = PATTERN.subn(replace_block, text)
    return new_text, count > 0


def iter_files(paths: Iterable[Path]) -> Iterator[Path]:
    for path in paths:
        if path.is_dir():
            yield from path.rglob("*.c")
        elif path.suffix == ".c":
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline internalPush expansions."
    )
    parser.add_argument("paths", nargs="+", help="Files or directories to process.")
    args = parser.parse_args(argv)

    any_changes = False
    for source in iter_files(Path(p) for p in args.paths):
        original = source.read_text()
        updated, changed = process_text(original)
        if changed:
            source.write_text(updated)
            print(f"Updated {source}", file=sys.stderr)
            any_changes = True
    return 0 if any_changes else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
