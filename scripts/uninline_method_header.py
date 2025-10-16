#!/usr/bin/env python3
"""
Replace hand-inlined methodHeaderOf expansions with methodHeaderOf() calls.

The generated interpreter sources often spell out the logic of
StackInterpreter>>#methodHeaderOf: inline as

    /* begin methodHeaderOf: */
    assert(isCompiledMethod(obj));
    header = fetchPointerofObject(HeaderIndex, obj);

as well as the comma-expression variant used inside conditional expressions:

    /* begin methodHeaderOf: */
    assert(isCompiledMethod(obj)),
    (header = fetchPointerofObject(HeaderIndex, obj)),

This script removes those inlined blocks and rewrites the fetch call to the
shared helper methodHeaderOf(), relying on that helper to perform the asserts.
"""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import Iterable

COMMENT_MARKER = "/* begin methodHeaderOf: */"
ASSERT_FRAGMENT = "assert(isCompiledMethod("
FETCH_NAME = "fetchPointerofObject"


def replace_fetch_call(line: str) -> tuple[str, bool]:
    """Return line with fetchPointerofObject(..., X) swapped for methodHeaderOf(X)."""

    call_start = line.find(FETCH_NAME)
    if call_start == -1:
        return line, False

    open_paren = line.find("(", call_start)
    if open_paren == -1:
        return line, False

    depth = 0
    close_paren = -1
    for idx in range(open_paren, len(line)):
        ch = line[idx]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0:
                close_paren = idx
                break
    if close_paren == -1:
        return line, False

    args = line[open_paren + 1 : close_paren]
    depth = 0
    comma_idx = -1
    for idx, ch in enumerate(args):
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        elif ch == "," and depth == 0:
            comma_idx = idx
            break

    if comma_idx == -1:
        return line, False

    first_arg = args[:comma_idx].strip()
    second_arg = args[comma_idx + 1 :].strip()

    if first_arg != "HeaderIndex":
        return line, False

    replacement = f"methodHeaderOf({second_arg})"
    new_line = line[:call_start] + replacement + line[close_paren + 1 :]
    return new_line, True


def process_text(text: str) -> tuple[str, bool]:
    lines = text.splitlines(keepends=True)
    output: list[str] = []
    i = 0
    changed = False

    while i < len(lines):
        line = lines[i]
        if COMMENT_MARKER in line:
            if i + 2 < len(lines) and ASSERT_FRAGMENT in lines[i + 1]:
                assign_line = lines[i + 2]
                new_assign, did_replace = replace_fetch_call(assign_line)
                if did_replace:
                    output.append(new_assign)
                    i += 3
                    changed = True
                    continue
        output.append(line)
        i += 1

    return "".join(output), changed


def iter_sources(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            for candidate in path.rglob("*.c"):
                if candidate.name == "methodHeaderOf.c":
                    continue
                yield candidate
        elif path.suffix == ".c" and path.name != "methodHeaderOf.c":
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline methodHeaderOf blocks to methodHeaderOf() calls."
    )
    parser.add_argument("paths", nargs="+", help="Source files or directories to process.")
    args = parser.parse_args(argv)

    any_changed = False
    for source in iter_sources(Path(p) for p in args.paths):
        original = source.read_text()
        rewritten, changed = process_text(original)
        if changed:
            source.write_text(rewritten)
            print(f"Updated {source}", file=sys.stderr)
            any_changed = True

    return 0 if any_changed else 0


if __name__ == "__main__":
    import sys

    sys.exit(main(sys.argv[1:]))
