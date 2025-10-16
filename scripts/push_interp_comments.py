#!/usr/bin/env python3
"""
Move the descriptive comments that precede each `#include "interp/..."` directive
from `interp.c` into the corresponding per-function file.

The script scans `interp.c` looking for contiguous blocks of C comments
immediately before an include of the form `#include "interp/<name>.c"`. Each
comment block is removed from `interp.c` and inserted near the top of the
included file (just after the existing banner comment, if present).
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import Dict, List


BLOCK_COMMENT = r"/\*[^*]*\*+(?:[^/*][^*]*\*+)*/"
LINE_COMMENT = r"//[^\n]*\n"

COMMENT_INCLUDE_RE = re.compile(
    rf"(?P<comment>(?:[ \t]*(?:{BLOCK_COMMENT}|{LINE_COMMENT})\s*)+)"
    r"(?P<include>[ \t]*#include\s+\"interp/(?P<file>[^\"]+)\")",
    re.DOTALL,
)


def extract_comment_map(content: str) -> tuple[str, Dict[str, List[str]]]:
    """Remove comment blocks from content and map them to include targets."""

    comment_map: Dict[str, List[str]] = {}
    parts: List[str] = []
    cursor = 0

    for match in COMMENT_INCLUDE_RE.finditer(content):
        start = match.start("comment")
        end = match.end("include")
        prefix = content[cursor:start]
        parts.append(prefix)
        if parts and not parts[-1].endswith("\n"):
            parts[-1] = parts[-1] + "\n"
        include_line = match.group("include")
        parts.append(include_line)
        if not include_line.endswith("\n"):
            parts.append("\n")

        cursor = end

        filename = match.group("file")
        comment = match.group("comment")
        if comment.strip():
            comment_map.setdefault(filename, []).append(comment)

    parts.append(content[cursor:])
    new_content = "".join(parts)
    return new_content, comment_map


def insert_comment(target_path: Path, comment_blocks: List[str]) -> None:
    """Insert the comment blocks near the top of the target file."""

    content = target_path.read_text(encoding="utf-8")
    blocks = []
    for block in comment_blocks:
        normalized = block.rstrip()
        if not normalized:
            continue
        # Ensure a blank line after the inserted comment block.
        blocks.append(normalized + "\n\n")
    if not blocks:
        return

    insertion = "".join(blocks)
    if insertion.strip() in content:
        return

    lines = content.splitlines(keepends=True)
    insert_at = 0

    # Skip leading blank lines.
    while insert_at < len(lines) and lines[insert_at].strip() == "":
        insert_at += 1

    # Skip the banner comment if present.
    if insert_at < len(lines) and lines[insert_at].lstrip().startswith("/*"):
        while insert_at < len(lines):
            if "*/" in lines[insert_at]:
                insert_at += 1
                break
            insert_at += 1
        # Skip blank lines after the banner.
        while insert_at < len(lines) and lines[insert_at].strip() == "":
            insert_at += 1

    new_lines = lines[:insert_at] + [insertion] + lines[insert_at:]
    target_path.write_text("".join(new_lines), encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "interp_file",
        type=Path,
        help="Path to the include-based interp.c file.",
    )
    parser.add_argument(
        "--functions-dir",
        type=Path,
        help="Directory containing the per-function sources "
        "(default: alongside interp.c in 'interp/').",
    )
    args = parser.parse_args()

    interp_path = args.interp_file.resolve()
    if not interp_path.exists():
        raise SystemExit(f"{interp_path} does not exist.")

    functions_dir = (
        args.functions_dir.resolve()
        if args.functions_dir
        else interp_path.parent / "interp"
    )
    if not functions_dir.is_dir():
        raise SystemExit(f"Functions directory {functions_dir} not found.")

    content = interp_path.read_text(encoding="utf-8")
    updated_content, comment_map = extract_comment_map(content)
    interp_path.write_text(updated_content, encoding="utf-8")

    for filename, blocks in comment_map.items():
        target = functions_dir / filename
        if not target.exists():
            raise SystemExit(f"Included file {target} does not exist.")
        insert_comment(target, blocks)

    print(f"Moved {sum(len(v) for v in comment_map.values())} comment blocks into {functions_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
