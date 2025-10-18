#!/usr/bin/env python3
"""
Collapse hand-inlined storePointer:ofObject:withValue: sequences into calls to
storePointerofObjectwithValue(fieldIndex, object, value).

The generated Spur interpreter sources expand the helper in-place with a block
of assertions, remembered-set maintenance, and the final longAtput store.
This script recognises that pattern and rewrites it to a single helper call,
preserving indentation.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable, List

BLOCK_RE = re.compile(
    r"(?P<indent>^[ \t]*)/\* begin storePointer:ofObject:withValue: \*/\s*"
    r"(?P<body>.*?)"
    r"(?P=indent)/\* most stores into young objects \*/\s*"
    r"(?P=indent)(?P<prefix>(?:return\s+|\(\(void\)\()?)longAtput\((?P<longcall>.*?\))"
    r"(?P<suffix>\)\))?;(?P<tail>\s*)",
    re.DOTALL | re.MULTILINE,
)


def split_args(arg_text: str) -> List[str]:
    """Split a comma-separated argument list while respecting nested parens."""
    args: List[str] = []
    current: List[str] = []
    depth = 0
    idx = 0
    length = len(arg_text)
    while idx < length:
        ch = arg_text[idx]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth = max(depth - 1, 0)
        elif ch == "," and depth == 0:
            args.append("".join(current).strip())
            current = []
            idx += 1
            continue
        current.append(ch)
        idx += 1
    if current:
        args.append("".join(current).strip())
    return args


def extract_args(block: str) -> tuple[str, str, str] | None:
    match = re.search(
        r"assert\s*\(\s*validStorePointerArgs\s*\(\s*(?P<args>.*?)\s*\)\s*\)\s*;",
        block,
        re.DOTALL,
    )
    if not match:
        return None
    parts = split_args(match.group("args"))
    if len(parts) != 3:
        return None
    return parts[0], parts[1], parts[2]


def replace_block(match: re.Match[str]) -> str:
    block = match.group("body")
    args = extract_args(block)
    if not args:
        return match.group(0)
    field, obj, value = args
    indent = match.group("indent")
    prefix = match.group("prefix")
    suffix = match.group("suffix") or ""
    tail = match.group("tail")
    return (
        f"{indent}{prefix}storePointerofObjectwithValue({field}, {obj}, {value})"
        f"{suffix};{tail}"
    )


def process_text(text: str) -> tuple[str, bool]:
    new_text, count = BLOCK_RE.subn(replace_block, text)
    return new_text, count > 0


def iter_source_files(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            yield from (
                p
                for p in path.rglob("*.c")
                if p.name not in {"storePointerofObjectwithValue.c"}
            )
        elif path.suffix == ".c" and path.name != "storePointerofObjectwithValue.c":
            yield path


def main(argv: List[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Replace inline storePointer:ofObject:withValue: blocks with helper calls."
    )
    parser.add_argument("paths", nargs="+", help="Files or directories to process")
    args = parser.parse_args(argv)

    any_changed = False
    for source in iter_source_files(Path(p) for p in args.paths):
        original = source.read_text()
        updated, changed = process_text(original)
        if changed:
            source.write_text(updated)
            print(f"Updated {source}", file=sys.stderr)
            any_changed = True
    return 0 if any_changed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
