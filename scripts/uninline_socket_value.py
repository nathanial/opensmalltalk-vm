#!/usr/bin/env python3
"""
Replace hand-inlined socketValueOf: sequences with calls to the shared helper.

The generated SocketPlugin sources expand socketValueOf: as:

    /* begin socketValueOf: */
    if ((isBytes(expr))
     && ((byteSizeOf(expr)) == (sizeof(SQSocket)))) {
        target = ((SocketPtr) (firstIndexableField(expr)));
    }
    else {
        primitiveFailFor(PrimErrBadArgument);
        target = ((SocketPtr) null);
    }

This script collapses each block to:

    target = socketValueOf(expr);
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable, Iterator

BLOCK_RE = re.compile(
    r"(?P<indent>^[ \t]*)/\* begin socketValueOf: \*/\s*\n"
    r"(?P=indent)if\s*\(\s*\(\s*isBytes\(\s*(?P<expr>[^)]+)\s*\)\)\s*\n"
    r"(?P=indent)[ \t]*&&\s*\(\s*\(byteSizeOf\(\s*(?P<expr2>[^)]+)\s*\)\)\s*==\s*\(sizeof\(SQSocket\)\)\)\)\s*\{\s*\n"
    r"(?P=indent)[ \t]*(?P<target>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*\(\(SocketPtr\)\s*\(firstIndexableField\(\s*(?P<expr3>[^)]+)\s*\)\)\);\s*\n"
    r"(?P=indent)\}\s*\n"
    r"(?P=indent)else\s*\{\s*\n"
    r"(?P=indent)[ \t]*primitiveFailFor\(PrimErrBadArgument\);\s*\n"
    r"(?P=indent)[ \t]*(?P=target)\s*=\s*\(\(SocketPtr\)\s*null\);\s*\n"
    r"(?P=indent)\}\s*",
    re.MULTILINE,
)


def iter_sources(paths: Iterable[Path]) -> Iterator[Path]:
    for path in paths:
        if path.is_dir():
            yield from path.rglob("*.c")
        elif path.suffix == ".c":
            yield path


def replace_blocks(text: str, path: Path) -> tuple[str, bool]:
    def _repl(match: re.Match[str]) -> str:
        expr = match.group("expr").strip()
        expr2 = match.group("expr2").strip()
        expr3 = match.group("expr3").strip()
        if not (expr == expr2 == expr3):
            raise RuntimeError(
                f"socketValueOf block argument mismatch in {path}: "
                f"{expr!r}, {expr2!r}, {expr3!r}"
            )
        target = match.group("target")
        indent = match.group("indent")
        return f"{indent}{target} = socketValueOf({expr});\n"

    updated, count = BLOCK_RE.subn(lambda m: _repl(m), text)
    return updated, count > 0


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Replace inline socketValueOf blocks with helper calls."
    )
    parser.add_argument("paths", nargs="+", help="Files or directories to process.")
    args = parser.parse_args(argv)

    any_changes = False
    for source in iter_sources(Path(p) for p in args.paths):
        original = source.read_text()
        updated, changed = replace_blocks(original, source)
        if changed:
            source.write_text(updated)
            print(f"Updated {source}", file=sys.stderr)
            any_changes = True
    return 0 if any_changes else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
