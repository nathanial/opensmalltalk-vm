#!/usr/bin/env python3
"""
Remove trivial `goto` statements that immediately jump to the next label.

This pass looks for lines of the form `goto foo;` where the next
non-empty, non-comment line is `foo:` (or `foo:;`). In such cases the
goto is redundant and can be deleted safely.
"""
from __future__ import annotations

import argparse
import pathlib
import re
from typing import Iterable


GOTO_RE = re.compile(r"\bgoto\s+([A-Za-z_][A-Za-z0-9_]*)\s*;")


def iter_c_files(paths: Iterable[pathlib.Path]) -> Iterable[pathlib.Path]:
    for path in paths:
        if path.is_dir():
            yield from iter_c_files(path.iterdir())
        elif path.suffix == ".c":
            yield path


def should_remove_goto(lines: list[str], index: int, label: str) -> bool:
    j = index + 1
    while j < len(lines):
        stripped = lines[j].strip()
        if not stripped or stripped.startswith("/*") or stripped.startswith("//"):
            j += 1
            continue
        return stripped in {f"{label}:", f"{label}:;"}
    return False


def process_file(path: pathlib.Path) -> bool:
    original = path.read_text()
    lines = original.splitlines()
    had_trailing_newline = original.endswith("\n")
    remove_indices: set[int] = set()

    for i, line in enumerate(lines):
        match = GOTO_RE.search(line)
        if not match:
            continue
        label = match.group(1)
        if should_remove_goto(lines, i, label):
            remove_indices.add(i)

    if not remove_indices:
        return False

    new_lines = [
        "" if i in remove_indices else line for i, line in enumerate(lines)
    ]
    content = "\n".join(new_lines)
    if had_trailing_newline:
        content += "\n"
    path.write_text(content)
    return True


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Remove trivial goto statements that jump to the next label."
    )
    parser.add_argument(
        "paths",
        nargs="*",
        type=pathlib.Path,
        default=[pathlib.Path("src")],
        help="Files or directories to process (default: src).",
    )
    args = parser.parse_args()

    files = list(iter_c_files(args.paths))
    updated = 0
    for path in files:
        if process_file(path):
            updated += 1
    print(f"Updated {updated} files.")


if __name__ == "__main__":
    main()
