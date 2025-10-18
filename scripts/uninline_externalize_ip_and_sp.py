#!/usr/bin/env python3
"""
Replace hand-inlined externalizeIPandSP blocks with calls to the shared helper.

The generated interpreter sources expand the helper as:

    /* begin externalizeIPandSP */
    instructionPointer = ((usqInt)localIP);
    stackPointer = localSP;
    framePointer = localFP;

This script collapses the block into:

    externalizeIPandSP(localIP, localSP, localFP);
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable, Iterator

BLOCK_RE = re.compile(
    r"(?P<indent>^[ \t]*)/\* begin externalizeIPandSP \*/[ \t]*\n"
    r"(?P=indent)instructionPointer\s*=\s*\(\(\s*usqInt\s*\)\s*localIP\s*\);[ \t]*\n"
    r"(?P=indent)stackPointer\s*=\s*localSP\s*;[ \t]*\n"
    r"(?P=indent)framePointer\s*=\s*localFP\s*;[ \t]*\n?",
    re.MULTILINE,
)


def iter_sources(paths: Iterable[Path]) -> Iterator[Path]:
    for path in paths:
        if path.is_dir():
            yield from path.rglob("*.c")
        elif path.suffix == ".c":
            yield path


def replace_blocks(text: str) -> tuple[str, bool]:
    def _repl(match: re.Match[str]) -> str:
        indent = match.group("indent")
        return f"{indent}externalizeIPandSP(localIP, localSP, localFP);\n"

    updated, count = BLOCK_RE.subn(_repl, text)
    return updated, count > 0


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Collapse inline externalizeIPandSP blocks into helper calls."
    )
    parser.add_argument("paths", nargs="+", help="Files or directories to process.")
    args = parser.parse_args(argv)

    any_changes = False
    for source in iter_sources(Path(p) for p in args.paths):
        original = source.read_text()
        updated, changed = replace_blocks(original)
        if changed:
            source.write_text(updated)
            print(f"Updated {source}", file=sys.stderr)
            any_changes = True
    return 0 if any_changes else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
