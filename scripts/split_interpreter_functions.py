#!/usr/bin/env python3
"""
Split every function definition from a monolithic C source file into its own file.

The script uses libclang to parse the translation unit, finds all function
definitions that originate from the requested source file, and writes each of
them to `OUTPUT_DIR/<function_name>.c`.  A short banner comment is added to
each generated file to retain provenance information.

Example usage:

    ./scripts/split_interpreter_functions.py src/spur64.stack/interp.c \
        --output-dir src/spur64.stack/interp \
        --libclang /Library/Developer/CommandLineTools/usr/lib/libclang.dylib \
        -- -Iinclude -Isrc -Isrc/spur64.stack -Iplatforms/Cross/vm \
           -Iplatforms/unix/vm -Iscripts/clang_stubs \
           -DUSE_GLOBAL_STRUCT=0 -DUSE_GLOBAL_STRUCT_REG=0 \
           -DLSB_FIRST=1 -DHAVE_CONFIG_H=1

The underlying source file is left untouched; the generated files serve as
building blocks for further refactoring.
"""

from __future__ import annotations

import argparse
import os
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, List, Sequence

from clang import cindex


@dataclass(frozen=True)
class FunctionSlice:
    name: str
    start_offset: int
    end_offset: int
    start_line: int


def configure_libclang(explicit_path: str | None) -> None:
    """Configure libclang shared library resolution."""

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


def parse_translation_unit(
    source_path: Path,
    clang_args: Sequence[str],
) -> cindex.TranslationUnit:
    """Parse the translation unit for `source_path`."""

    index = cindex.Index.create()
    options = cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD
    try:
        tu = index.parse(str(source_path), args=list(clang_args), options=options)
    except cindex.TranslationUnitLoadError as exc:
        raise SystemExit(f"libclang failed to parse {source_path}: {exc}") from exc

    diagnostics = [
        diag
        for diag in tu.diagnostics
        if diag.severity >= cindex.Diagnostic.Error
    ]
    if diagnostics:
        formatted = "\n".join(f"{diag.location}: {diag.spelling}" for diag in diagnostics)
        raise SystemExit(
            f"libclang reported errors while parsing {source_path}:\n{formatted}"
        )
    return tu


def collect_function_slices(
    tu: cindex.TranslationUnit,
    source_path: Path,
) -> List[FunctionSlice]:
    """Return all function definitions originating from `source_path`."""

    functions: List[FunctionSlice] = []
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
                FunctionSlice(
                    name=node.spelling or f"anon_func_{extent.start.line}",
                    start_offset=extent.start.offset,
                    end_offset=extent.end.offset,
                    start_line=extent.start.line,
                )
            )
            return

        for child in node.get_children():
            if child.location.file and child.location.file.name == source_str:
                visit(child)

    visit(tu.cursor)
    functions.sort(key=lambda info: info.start_offset)
    return functions


def sanitize_filename(name: str) -> str:
    """Convert a function name to a filesystem-safe stem."""

    stem = re.sub(r"[^0-9A-Za-z_]", "_", name)
    return stem or "function"


def write_function_files(
    *,
    functions: Iterable[FunctionSlice],
    source_text: str,
    source_path: Path,
    output_dir: Path,
    overwrite: bool,
) -> List[Path]:
    """Emit each function slice to its own file inside `output_dir`."""

    output_dir.mkdir(parents=True, exist_ok=True)
    written: List[Path] = []
    for info in functions:
        stem = sanitize_filename(info.name)
        candidate = output_dir / f"{stem}.c"
        if candidate.exists() and not overwrite:
            suffix = 1
            while True:
                alternate = output_dir / f"{stem}_{suffix}.c"
                if not alternate.exists():
                    candidate = alternate
                    break
                suffix += 1

        body = source_text[info.start_offset:info.end_offset]
        banner = (
            f"/* Extracted from {source_path.name}:{info.start_line} "
            f"(function {info.name}). */\n\n"
        )
        candidate.write_text(banner + body, encoding="utf-8")
        written.append(candidate)
    return written


def main(argv: Sequence[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="Path to the monolithic C source file.")
    parser.add_argument(
        "--output-dir",
        type=Path,
        help="Directory to place the generated function files. "
        "Defaults to <SOURCE_DIR>/interp.",
    )
    parser.add_argument(
        "--libclang",
        help="Explicit path to libclang shared library. "
        "If omitted, LIBCLANG_PATH is consulted.",
    )
    parser.add_argument(
        "--overwrite",
        action="store_true",
        help="Allow overwriting existing files in the output directory.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Collect functions but do not write any files.",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Print each function name as it is processed.",
    )

    args, extra = parser.parse_known_args(argv)
    clang_args: List[str] = list(extra)
    if clang_args and clang_args[0] == "--":
        clang_args = clang_args[1:]

    configure_libclang(args.libclang)

    source_path = args.source.resolve()
    if not source_path.exists():
        raise SystemExit(f"Source file {source_path} does not exist.")

    output_dir = (
        args.output_dir.resolve()
        if args.output_dir
        else (source_path.parent / "interp")
    )

    source_text = source_path.read_text(encoding="utf-8")
    tu = parse_translation_unit(source_path, clang_args)
    functions = collect_function_slices(tu, source_path)

    if not functions:
        print("No function definitions found; nothing to do.")
        return 0

    if args.verbose:
        for info in functions:
            print(f"{info.name} @ line {info.start_line}")

    print(f"Identified {len(functions)} function definitions.")

    if args.dry_run:
        print("Dry run enabled; no files written.")
        return 0

    written = write_function_files(
        functions=functions,
        source_text=source_text,
        source_path=source_path,
        output_dir=output_dir,
        overwrite=args.overwrite,
    )
    print(f"Wrote {len(written)} files into {output_dir}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
