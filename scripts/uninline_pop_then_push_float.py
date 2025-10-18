#!/usr/bin/env python3
"""
Replace hand-inlined pop:thenPushFloat: blocks with popthenPushFloat calls.

The generated sources spell the helper inline as:

    /* begin pop:thenPushFloat: */
    longAtput((sp = stackPointer + (<expr> * BytesPerWord)),
              floatObjectOf(result));
    stackPointer = sp;

This script rewrites those sequences to

    /* begin pop:thenPushFloat: */
    popthenPushFloat((<expr> + 1), result);
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import Iterable

COMMENT = "/* begin pop:thenPushFloat: */"
STACK_POINTER_LINE = re.compile(r"^\s*stackPointer\s*=\s*sp;\s*$")
MULTIPLIER_RE = re.compile(
    r"stackPointer\s*\+\s*\(\s*(?P<mul>.+?)\s*\*\s*BytesPerWord\s*\)"
)


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
            long_line = lines[i + 1]
            multiplier = extract_multiplier(long_line)
            j = i + 1
            value_lines = [lines[j]]
            while j + 1 < len(lines) and ");" not in value_lines[-1]:
                j += 1
                value_lines.append(lines[j])
            value = extract_value(value_lines)
            stack_line_index = j + 1
            if (
                multiplier
                and value
                and stack_line_index < len(lines)
                and STACK_POINTER_LINE.match(lines[stack_line_index].strip("\n"))
            ):
                indent = long_line[: len(long_line) - len(long_line.lstrip())]
                replacement = (
                    f"{indent}popthenPushFloat(({multiplier}) + 1, {value});\n"
                )
                output.append(line)
                output.append(replacement)
                i = stack_line_index + 1
                changed = True
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
                if candidate.name != "popthenPushFloat.c"
            )
        elif path.suffix == ".c" and path.name != "popthenPushFloat.c":
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline pop:thenPushFloat: blocks with popthenPushFloat calls."
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
