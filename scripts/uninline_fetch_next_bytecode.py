#!/usr/bin/env python3
"""
Replace inline fetchNextBytecode expansions with calls to the shared helper.

The generated interpreter code often emits:

    /* begin fetchNextBytecode */
    currentBytecode = (byteAt(++localIP)) + bytecodeSetSelector;

or simply the assignment line without the preceding comment. This script
collapses either form to:

    currentBytecode = fetchNextBytecode(&localIP, bytecodeSetSelector);

leaving surrounding indentation intact.
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
    (?:/\* \s*begin \s+ fetchNextBytecode \s*\*/\s*\r?\n(?P=indent))?
    currentBytecode
    \s*=\s*
    \(byteAt\(
    \s*\+\+\s*localIP
    \s*\)\)
    \s*\+\s*
    bytecodeSetSelector
    \s*;
    (?P<tail>[ \t]*\r?\n?)
    """,
    re.MULTILINE | re.VERBOSE,
)


def replace_match(match: re.Match[str]) -> str:
    indent = match.group("indent")
    tail = match.group("tail") or "\n"
    return f"{indent}currentBytecode = fetchNextBytecode(&localIP, bytecodeSetSelector);{tail}"


def process_text(text: str) -> Tuple[str, bool]:
    new_text, count = PATTERN.subn(replace_match, text)
    return new_text, count > 0


def iter_files(paths: Iterable[Path]) -> Iterator[Path]:
    for path in paths:
        if path.is_dir():
            yield from path.rglob("*.c")
        elif path.suffix == ".c":
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline fetchNextBytecode expansions."
    )
    parser.add_argument("paths", nargs="+", help="Files or directories to process.")
    args = parser.parse_args(argv)

    any_changes = False
    for path in iter_files(Path(p) for p in args.paths):
        original = path.read_text()
        updated, changed = process_text(original)
        if changed:
            path.write_text(updated)
            print(f"Updated {path}", file=sys.stderr)
            any_changes = True
    return 0 if any_changes else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
