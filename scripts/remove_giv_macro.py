#!/usr/bin/env python3
"""
Rewrite helper that removes the StackInterpreter GIV(...) macro wrappers.

This script parses the given C translation unit with libclang, identifies every
invocation of the `GIV` macro in the source file, and replaces the invocation
with the textual contents inside the parentheses.  The transformation is
driven by libclang so that macro expansions in included headers are ignored and
only the invocations that originate from the requested file are rewritten.

Example usage:

    ./scripts/remove_giv_macro.py src/spur64.stack/interp.c -- -Iinclude -DSQ_USE_GLOBAL_STRUCT=0

By default the file is rewritten in place.  Use `--dry-run` to preview the
changes without modifying the file, or `--output` to write the transformed
content elsewhere.
"""

from __future__ import annotations

import argparse
import os
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, List, Sequence, Tuple

from clang import cindex


@dataclass(frozen=True)
class Replacement:
    """Represents a replacement to be applied to the source text."""

    start: int
    end: int
    text: str

    def __post_init__(self) -> None:
        if self.end < self.start:
            raise ValueError(f"Invalid replacement range: {self.start}-{self.end}")


def configure_libclang(explicit_path: str | None) -> None:
    """Configure libclang shared library resolution."""

    if explicit_path:
        cindex.Config.set_library_file(explicit_path)
        return

    env_path = os.environ.get("LIBCLANG_PATH")
    if env_path:
        candidate = Path(env_path)
        if candidate.is_file():
            cindex.Config.set_library_file(str(candidate))
        elif candidate.is_dir():
            # Probe typical library names inside the directory.
            for name in ("libclang.dylib", "libclang.so", "libclang.dll"):
                lib_path = candidate / name
                if lib_path.exists():
                    cindex.Config.set_library_file(str(lib_path))
                    break


def parse_translation_unit(
    source_path: Path, clang_args: Sequence[str], detailed: bool = True
) -> cindex.TranslationUnit:
    """Parse the translation unit for `source_path` using libclang."""

    index = cindex.Index.create()
    parse_options = cindex.TranslationUnit.PARSE_NONE
    if detailed:
        parse_options |= cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD

    try:
        tu = index.parse(
            str(source_path),
            args=list(clang_args),
            options=parse_options,
        )
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


def collect_giv_replacements(
    tu: cindex.TranslationUnit, source_path: Path, source_text: str
) -> List[Replacement]:
    """Collect replacements that strip GIV(...) macro invocations."""

    replacements: List[Replacement] = []
    source_file = tu.get_file(str(source_path))
    if source_file is None:
        raise SystemExit(f"Translation unit does not contain file {source_path}")

    tokens = list(tu.get_tokens(extent=tu.cursor.extent))
    i = 0
    token_count = len(tokens)
    while i < token_count:
        token = tokens[i]
        if token.spelling != "GIV" or token.location.file is None:
            i += 1
            continue
        if token.location.file.name != str(source_path):
            i += 1
            continue

        # Skip macro definitions: look backwards for a 'define' token without
        # intervening newline.
        prev = tokens[i - 1] if i > 0 else None
        if prev and prev.spelling == "define":
            i += 1
            continue

        # Expect the next token to be '('.
        if i + 1 >= token_count or tokens[i + 1].spelling != "(":
            i += 1
            continue

        depth = 0
        j = i + 1
        closing_index = None
        while j < token_count:
            current = tokens[j]
            if current.location.file is not None and current.location.file.name != str(
                source_path
            ):
                # Entered a different file (likely through a macro); abort.
                break
            if current.spelling == "(":
                depth += 1
            elif current.spelling == ")":
                depth -= 1
                if depth == 0:
                    closing_index = j
                    break
            j += 1

        if closing_index is None or depth != 0:
            # Unbalanced invocation; skip to avoid unsafe edits.
            i += 1
            continue

        open_paren = tokens[i + 1]
        close_paren = tokens[closing_index]

        start_offset = token.extent.start.offset
        end_offset = close_paren.extent.end.offset

        inner_start = open_paren.extent.end.offset
        inner_end = close_paren.extent.start.offset

        # Extract the original inner text preserving formatting.
        inner_text = source_text[inner_start:inner_end]
        replacement_text = inner_text

        replacements.append(Replacement(start=start_offset, end=end_offset, text=replacement_text))
        i = closing_index + 1
    return replacements


def apply_replacements(source_text: str, replacements: Sequence[Replacement]) -> str:
    """Return the updated text after applying replacements."""

    if not replacements:
        return source_text

    replacements = sorted(replacements, key=lambda repl: repl.start)

    updated_parts: List[str] = []
    cursor = 0
    for repl in replacements:
        if repl.start < cursor:
            raise SystemExit("Overlapping replacements detected; aborting.")
        updated_parts.append(source_text[cursor:repl.start])
        updated_parts.append(repl.text)
        cursor = repl.end
    updated_parts.append(source_text[cursor:])
    return "".join(updated_parts)


def write_output(target_path: Path, content: str) -> None:
    target_path.write_text(content, encoding="utf-8")


def main(argv: Sequence[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="Path to the C source file to rewrite.")
    parser.add_argument(
        "--output",
        type=Path,
        help="Optional output path. Defaults to rewriting SOURCE in place.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show the number of replacements but do not write changes.",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Print details about each replacement range.",
    )
    parser.add_argument(
        "--libclang",
        help="Explicit path to libclang shared library. "
        "If omitted, LIBCLANG_PATH is consulted.",
    )
    parser.add_argument(
        "--backup",
        action="store_true",
        help="When rewriting in place, keep a .bak copy of the original file.",
    )

    args, extra = parser.parse_known_args(argv)
    clang_args: List[str] = list(extra)
    if clang_args and clang_args[0] == "--":
        clang_args = clang_args[1:]

    configure_libclang(args.libclang)

    source_path = args.source.resolve()
    if not source_path.exists():
        raise SystemExit(f"Source file {source_path} does not exist.")

    source_text = source_path.read_text(encoding="utf-8")
    tu = parse_translation_unit(source_path, clang_args)
    replacements = collect_giv_replacements(tu, source_path, source_text)

    if not replacements:
        print("No GIV(...) invocations found; no changes made.")
        return 0

    if args.verbose:
        for repl in replacements:
            print(
                f"rewrite [{repl.start}, {repl.end}) -> {repl.text!r}",
                file=sys.stderr,
            )
    print(f"Found {len(replacements)} GIV(...) invocations.")

    if args.dry_run:
        print("Dry run enabled; skipping write.")
        return 0

    new_text = apply_replacements(source_text, replacements)
    output_path = args.output.resolve() if args.output else source_path

    if args.backup and not args.output:
        backup_path = source_path.with_suffix(source_path.suffix + ".bak")
        backup_path.write_text(source_text, encoding="utf-8")
        print(f"Backup written to {backup_path}")

    write_output(output_path, new_text)
    print(f"Wrote updated source to {output_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
