#!/usr/bin/env python3
"""
Generate a variant of a monolithic C source file where each function body is
replaced by an `#include` directive that points at a per-function source file.

The script expects the per-function files to already exist (e.g., created by
`split_interpreter_functions.py`) and will fail if any expected file is missing.
"""

from __future__ import annotations

import argparse
import os
from pathlib import Path
from typing import Dict, Iterable, List, Sequence, Tuple

from clang import cindex

# Reuse minimal helper dataclass style from the splitter script.


def configure_libclang(explicit_path: str | None) -> None:
    if explicit_path:
        cindex.Config.set_library_file(explicit_path)
        return

    env_path = os.environ.get("LIBCLANG_PATH")
    if not env_path:
        return

    candidate = Path(env_path)
    if candidate.is_file():
        cindex.Config.set_library_file(str(candidate))
    elif candidate.is_dir():
        for name in ("libclang.dylib", "libclang.so", "libclang.dll"):
            shared = candidate / name
            if shared.exists():
                cindex.Config.set_library_file(str(shared))
                break


def parse_translation_unit(source_path: Path, clang_args: Sequence[str]) -> cindex.TranslationUnit:
    index = cindex.Index.create()
    options = cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD
    tu = index.parse(str(source_path), args=list(clang_args), options=options)
    diagnostics = [diag for diag in tu.diagnostics if diag.severity >= cindex.Diagnostic.Error]
    if diagnostics:
        formatted = "\n".join(f"{diag.location}: {diag.spelling}" for diag in diagnostics)
        raise SystemExit(
            f"libclang reported errors while parsing {source_path}:\n{formatted}"
        )
    return tu


def collect_function_slices(
    tu: cindex.TranslationUnit, source_path: Path
) -> List[Tuple[str, int, int]]:
    """Return (name, start_offset, end_offset) for each function defined in source."""

    functions: List[Tuple[str, int, int]] = []
    source_str = str(source_path)

    def visit(node: cindex.Cursor) -> None:
        if (
            node.kind == cindex.CursorKind.FUNCTION_DECL
            and node.is_definition()
            and node.location.file
            and node.location.file.name == source_str
        ):
            extent = node.extent
            functions.append(
                (
                    node.spelling or f"anon_func_{extent.start.line}",
                    extent.start.offset,
                    extent.end.offset,
                )
            )
            return

        for child in node.get_children():
            if child.location.file and child.location.file.name == source_str:
                visit(child)

    visit(tu.cursor)
    functions.sort(key=lambda item: item[1])
    return functions


def sanitize_filename(name: str) -> str:
    return "".join(ch if (ch.isalnum() or ch == "_") else "_" for ch in name) or "function"


def assigned_filename(stem: str, occurrence: int) -> str:
    if occurrence == 0:
        return f"{stem}.c"
    return f"{stem}_{occurrence}.c"


def build_include_mapping(
    functions: Iterable[Tuple[str, int, int]],
    output_dir: Path,
    include_prefix: Path,
) -> List[Tuple[int, int, str]]:
    mapping: List[Tuple[int, int, str]] = []
    include_prefix = Path(include_prefix)

    buckets: Dict[str, List[Tuple[int, str]]] = {}
    for path in sorted(output_dir.iterdir()):
        if not path.is_file() or path.suffix != ".c":
            continue
        base = path.stem
        stem = base
        occurrence = 0
        tail_sep = base.rsplit("_", 1)
        if len(tail_sep) == 2 and tail_sep[1].isdigit():
            stem = tail_sep[0]
            occurrence = int(tail_sep[1])
        buckets.setdefault(stem, []).append((occurrence, path.name))

    for entries in buckets.values():
        entries.sort(key=lambda item: item[0])

    for name, start, end in functions:
        stem = sanitize_filename(name)
        entries = buckets.get(stem)
        if not entries:
            raise SystemExit(
                f"Expected extracted function file for '{stem}' is missing in {output_dir}."
            )
        _, filename = entries.pop(0)
        include_rel = (include_prefix / filename).as_posix()
        mapping.append((start, end, include_rel))

    return mapping


def compose_source_with_includes(
    source_text: str,
    replacements: List[Tuple[int, int, str]],
) -> str:
    pieces: List[str] = []
    cursor = 0
    for start, end, include_rel in replacements:
        pieces.append(source_text[cursor:start])
        if pieces and not pieces[-1].endswith("\n"):
            pieces[-1] = pieces[-1] + "\n"
        pieces.append(f"#include \"{include_rel}\"\n")
        if not source_text[end:end + 1].startswith("\n"):
            pieces.append("\n")
            cursor = end
        else:
            cursor = end
    pieces.append(source_text[cursor:])
    return "".join(pieces)


def main(argv: Sequence[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="Path to the original interp.c file.")
    parser.add_argument(
        "--output",
        type=Path,
        help="Optional path for the generated file. Defaults to <source> with '.includes.c'.",
    )
    parser.add_argument(
        "--libclang",
        help="Explicit path to libclang. If omitted, LIBCLANG_PATH is used.",
    )
    parser.add_argument(
        "--functions-dir",
        type=Path,
        help="Directory containing the extracted function files (default: <source_dir>/interp).",
    )
    parser.add_argument(
        "--include-prefix",
        type=str,
        help=(
            "Path to use in generated #include directives (default: functions directory "
            "relative to the source file)."
        ),
    )

    args, extra = parser.parse_known_args(argv)
    clang_args: List[str] = list(extra)
    if clang_args and clang_args[0] == "--":
        clang_args = clang_args[1:]

    source_path = args.source.resolve()
    if not source_path.exists():
        raise SystemExit(f"Source file {source_path} does not exist.")

    functions_dir = (
        args.functions_dir.resolve()
        if args.functions_dir
        else source_path.parent / "interp"
    )
    if not functions_dir.is_dir():
        raise SystemExit(f"Functions directory {functions_dir} does not exist.")

    if args.include_prefix is not None:
        include_prefix = Path(args.include_prefix)
    else:
        include_prefix = Path(
            os.path.relpath(functions_dir, source_path.parent)
        )

    configure_libclang(args.libclang)

    source_text = source_path.read_text(encoding="utf-8")
    tu = parse_translation_unit(source_path, clang_args)
    functions = collect_function_slices(tu, source_path)
    if not functions:
        print("No function definitions found; nothing to replace.")
        return 0

    replacements = build_include_mapping(functions, functions_dir, include_prefix)
    rewritten = compose_source_with_includes(source_text, replacements)

    output_path = (
        args.output.resolve()
        if args.output
        else source_path.with_suffix(source_path.suffix + ".includes.c")
    )
    output_path.write_text(rewritten, encoding="utf-8")
    print(f"Wrote include-based source to {output_path}")
    return 0


if __name__ == "__main__":
    import sys

    sys.exit(main(sys.argv[1:]))
