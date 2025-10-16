#!/usr/bin/env python3
"""
Replace hand-inlined numSlotsOfAny: expansions with calls to numSlotsOfAny().

The generated interpreter sources often inline the logic for numSlotsOfAny: as
two statements:

    numSlotsUsqInt = byteAt((void *)(obj + (numSlotsFieldByteOffset())));
    numSlots = (numSlotsUsqInt == (numSlotsMask()) ? ... : numSlotsUsqInt);

This script rewrites those sequences to simply invoke the helper
`numSlotsOfAny(obj)`, and drops the now-unused `numSlotsUsqInt` declaration.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable

COMMENT = "/* begin numSlotsOfAny: */"
BLOCK_RE = re.compile(
    r"(?P<indent>^[ \t]*)(?:/\* begin numSlotsOfAny: \*/\s*)?"
    r"(?P=indent)numSlotsUsqInt\s*=\s*byteAt\(\s*\(void\s*\*\)\s*\(\s*"
    r"(?P<obj>.*?)\s*\+\s*\(numSlotsFieldByteOffset\(\)\)\s*\)\s*\)\s*;\s*"
    r"(?P=indent)(?P<dest>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*\(\s*numSlotsUsqInt\s*==\s*"
    r"\(numSlotsMask\(\)\)\s*\?\s*(?P<true>.*?)\s*:\s*numSlotsUsqInt\s*\)\s*;"
    r"(?P<tail>(?:[ \t]*\r?\n[ \t]*)*)",
    re.DOTALL | re.MULTILINE,
)
DECL_RE = re.compile(
    r"^[ \t]*usqInt\s+numSlotsUsqInt;\s*(?:\r?\n|$)",
    re.MULTILINE,
)


def replace_blocks(text: str) -> tuple[str, bool]:
    changed = False

    def _repl(match: re.Match[str]) -> str:
        nonlocal changed
        changed = True
        block_text = match.group(0)
        indent_line = next(
            (line for line in block_text.splitlines() if "numSlotsUsqInt" in line),
            "",
        )
        indent_len = len(indent_line) - len(indent_line.lstrip(" \t"))
        indent = indent_line[:indent_len]
        obj = match.group("obj").strip()
        dest = match.group("dest")
        tail = match.group("tail")
        if not tail.startswith(("\n", "\r")):
            tail = "\n" + tail
        return f"{indent}{dest} = numSlotsOfAny({obj});{tail}"

    new_text, count = BLOCK_RE.subn(_repl, text)
    return new_text, changed or count > 0


def remove_unused_decl(text: str) -> str:
    matches = list(re.finditer(r"\bnumSlotsUsqInt\b", text))
    if not matches:
        return text

    for match in matches:
        line_start = text.rfind("\n", 0, match.start()) + 1
        line_end = text.find("\n", match.end())
        if line_end == -1:
            line_end = len(text)
        line = text[line_start:line_end]
        if not re.fullmatch(r"[ \t]*usqInt\s+numSlotsUsqInt;\s*", line):
            return text

    return DECL_RE.sub("", text)


def process_text(text: str) -> tuple[str, bool]:
    replaced, changed = replace_blocks(text)
    cleaned = remove_unused_decl(replaced)
    return cleaned, changed or replaced != cleaned


def iter_source_files(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            yield from (
                p
                for p in path.rglob("*.c")
                if p.name != "numSlotsOfAny.c"
            )
        elif path.suffix == ".c" and path.name != "numSlotsOfAny.c":
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline numSlotsOfAny expansions to numSlotsOfAny() calls."
    )
    parser.add_argument(
        "paths", nargs="+", help="Source files or directories to process."
    )
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
