#!/usr/bin/env python3
"""Report call-site counts for B2DPlugin split function sources."""

from __future__ import annotations

import argparse
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Iterator


EXTENSIONS = {".c", ".h", ".mm", ".m"}
COMMENT_FUNC_RE = re.compile(r"\(function ([^)]+)\)")
DEFINE_RE_TEMPLATE = r"static\s+[^{{;]*\b{0}\s*\("
CALL_RE_TEMPLATE = r"\b{0}\s*\("


@dataclass(frozen=True)
class FunctionInfo:
    name: str
    path: Path


def iter_function_files(root: Path) -> Iterator[Path]:
    for candidate in sorted(root.iterdir()):
        if candidate.is_file() and candidate.suffix == ".c":
            yield candidate


def extract_function_name(path: Path) -> str:
    text = path.read_text(encoding="utf-8")
    comment_match = COMMENT_FUNC_RE.search(text)
    if comment_match:
        return comment_match.group(1)

    signature_match = re.search(r"static\s+\w[\w\s\*]*\s+(\w+)\s*\(", text)
    if signature_match:
        return signature_match.group(1)

    raise ValueError(f"Unable to determine function name from {path}")


def collect_function_infos(function_dir: Path) -> list[FunctionInfo]:
    infos: list[FunctionInfo] = []
    for file_path in iter_function_files(function_dir):
        name = extract_function_name(file_path)
        infos.append(FunctionInfo(name=name, path=file_path))
    return infos


def iter_source_files(root: Path) -> Iterable[Path]:
    for path in root.rglob("*"):
        if not path.is_file():
            continue
        if path.suffix in EXTENSIONS:
            yield path


def count_calls(name: str, definition: Path, sources: Iterable[Path]) -> int:
    call_re = re.compile(CALL_RE_TEMPLATE.format(re.escape(name)))
    define_re = re.compile(DEFINE_RE_TEMPLATE.format(re.escape(name)))
    total = 0
    for path in sources:
        text = path.read_text(encoding="utf-8", errors="ignore")
        matches = call_re.findall(text)
        if not matches:
            continue
        subtotal = len(matches)
        if path == definition:
            subtotal -= len(define_re.findall(text))
            if subtotal < 0:
                subtotal = 0
        total += subtotal
    return total


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--functions-dir",
        type=Path,
        default=Path("src/plugins/B2DPlugin/functions"),
        help="Directory that contains the generated per-function sources.",
    )
    parser.add_argument(
        "--search-root",
        type=Path,
        default=Path.cwd(),
        help="Root directory to scan for call sites (default: repo root).",
    )
    parser.add_argument(
        "--sort",
        choices={"name", "count"},
        default="count",
        help="Sort order for the output (default: count).",
    )

    args = parser.parse_args()
    function_dir = args.functions_dir.resolve()
    search_root = args.search_root.resolve()

    if not function_dir.is_dir():
        raise SystemExit(f"Functions directory {function_dir} does not exist")
    if not search_root.is_dir():
        raise SystemExit(f"Search root {search_root} does not exist")

    infos = collect_function_infos(function_dir)
    if not infos:
        raise SystemExit(f"No function files found in {function_dir}")

    sources = list(iter_source_files(search_root))

    results = []
    for info in infos:
        count = count_calls(info.name, info.path, sources)
        results.append((info.name, count))

    if args.sort == "count":
        results.sort(key=lambda item: (item[1], item[0]))
    else:
        results.sort(key=lambda item: item[0])

    for name, count in results:
        print(f"{count:4d}  {name}")


if __name__ == "__main__":
    main()
