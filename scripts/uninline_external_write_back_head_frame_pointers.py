#!/usr/bin/env python3
"""
Replace inline externalWriteBackHeadFramePointers blocks with calls to the helper.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import Iterable, List

COMMENT = "/* begin externalWriteBackHeadFramePointers */"
END_SENTINEL = "assert(pageListIsWellFormed());"
CALL = "externalWriteBackHeadFramePointers();\n"


def iter_sources(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            yield from path.rglob("*.c")
        elif path.suffix == ".c":
            yield path


def transform(lines: List[str], path: Path) -> List[str]:
    new_lines: List[str] = []
    i = 0
    changed = False

    while i < len(lines):
        line = lines[i]
        if COMMENT not in line:
            new_lines.append(line)
            i += 1
            continue

        indent = line[: len(line) - len(line.lstrip())]
        j = i + 1
        while j < len(lines) and END_SENTINEL not in lines[j]:
            j += 1
        if j == len(lines):
            raise RuntimeError(
                f"Failed to find end of externalWriteBackHeadFramePointers block in {path}:{i + 1}"
            )

        new_lines.append(f"{indent}{CALL}")

        next_index = j + 1
        if next_index < len(lines) and lines[next_index].strip() == "":
            new_lines.append(lines[next_index])
            next_index += 1

        i = next_index
        changed = True

    return new_lines if changed else lines


def process_file(path: Path) -> bool:
    original = path.read_text().splitlines(keepends=True)
    transformed = transform(original, path)
    if transformed != original:
        path.write_text("".join(transformed))
        print(f"Updated {path}", file=sys.stderr)
        return True
    return False


def main(argv: List[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Replace inline externalWriteBackHeadFramePointers blocks with helper calls."
    )
    parser.add_argument("paths", nargs="+", help="Files or directories to rewrite.")
    args = parser.parse_args(argv)

    any_changed = False
    for source in iter_sources(Path(p) for p in args.paths):
        if "externalWriteBackHeadFramePointers.c" in source.name:
            continue
        if process_file(source):
            any_changed = True

    return 0 if any_changed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
