#!/usr/bin/env python3
"""Strip the legacy NoDbgRegParms marker from generated interpreter fragments."""

from __future__ import annotations

import argparse
import pathlib
import re
from typing import Iterable


TOKEN_PATTERN = re.compile(r"\bNoDbgRegParms\b[ \t]*")


def iter_targets(root: pathlib.Path) -> Iterable[pathlib.Path]:
    """Yield every C source file under *root*."""
    for path in sorted(root.rglob("*.c")):
        if path.is_file():
            yield path


def strip_token(path: pathlib.Path) -> bool:
    """Remove NoDbgRegParms tokens from *path*; return True if modified."""
    original = path.read_text()
    updated = TOKEN_PATTERN.sub("", original)
    if updated != original:
        path.write_text(updated)
        return True
    return False


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Remove NoDbgRegParms tokens from generated interpreter sources."
    )
    parser.add_argument(
        "paths",
        nargs="*",
        type=pathlib.Path,
        default=[pathlib.Path("src/spur64.stack/interp")],
        help="Files or directories to process (defaults to the interp/ folder).",
    )
    args = parser.parse_args()

    modified = 0
    seen_files = 0
    for path in args.paths:
        if path.is_dir():
            for target in iter_targets(path):
                seen_files += 1
                if strip_token(target):
                    modified += 1
        elif path.suffix == ".c" and path.is_file():
            seen_files += 1
            if strip_token(path):
                modified += 1

    print(f"Processed {seen_files} files; updated {modified}.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
