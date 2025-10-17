#!/usr/bin/env python3
"""
Merge consecutive single-variable declarations of the same type within C functions.

The script uses libclang to parse each translation unit and rewrites runs of
`type var;` declarations into a single `type var1, var2, ...;` statement.
Only declarations without initializers that belong to the same lexical scope
are merged. Whitespace or comments between declarations prevent merging.
"""

from __future__ import annotations

import argparse
import os
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Sequence, Tuple

from clang import cindex


@dataclass(frozen=True)
class DeclCandidate:
    start: int
    end: int
    type_spelling: str
    name: str
    indent: str
    scope_key: Tuple[int, int]


@dataclass(frozen=True)
class Replacement:
    start: int
    end: int
    text: str


def configure_libclang(explicit: str | None) -> None:
    if explicit:
        cindex.Config.set_library_file(explicit)
        return
    env_path = os.environ.get("LIBCLANG_PATH")
    if not env_path:
        return
    candidate = Path(env_path)
    if candidate.is_file():
        cindex.Config.set_library_file(str(candidate))
    elif candidate.is_dir():
        for name in ("libclang.dylib", "libclang.so", "libclang.dll"):
            lib_path = candidate / name
            if lib_path.exists():
                cindex.Config.set_library_file(str(lib_path))
                break


def parse_tu(source: Path, extra_args: Sequence[str]) -> cindex.TranslationUnit:
    index = cindex.Index.create()
    options = cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD
    tu = index.parse(str(source), args=list(extra_args), options=options)
    diagnostics = [
        d for d in tu.diagnostics if d.severity >= cindex.Diagnostic.Error
    ]
    if diagnostics:
        formatted = "\n".join(f"{d.location}: {d.spelling}" for d in diagnostics)
        raise SystemExit(
            f"libclang reported errors while parsing {source}:\n{formatted}"
        )
    return tu


def compute_indent(text: str, offset: int) -> str:
    start = text.rfind("\n", 0, offset)
    if start == -1:
        start = 0
    else:
        start += 1
    return text[start:offset]


def gather_decl_candidates(
    tu: cindex.TranslationUnit, source: Path, source_text: str
) -> List[DeclCandidate]:
    file_obj = tu.get_file(str(source))
    candidates: List[DeclCandidate] = []

    def visit(cursor: cindex.Cursor) -> None:
        if cursor.location.file != file_obj:
            return
        if cursor.kind == cindex.CursorKind.DECL_STMT:
            parent = cursor.semantic_parent
            if not parent or parent.kind != cindex.CursorKind.COMPOUND_STMT:
                return

            var_decls = [
                child
                for child in cursor.get_children()
                if child.kind == cindex.CursorKind.VAR_DECL
                and child.location.file == file_obj
            ]
            if len(var_decls) != 1:
                return
            var = var_decls[0]
            if var.has_initializer:
                return
            if var.storage_class != cindex.StorageClass.NONE:
                return

            type_spelling = var.type.spelling
            start_offset = cursor.extent.start.offset
            end_offset = cursor.extent.end.offset
            indent = compute_indent(source_text, start_offset)
            scope = (parent.extent.start.offset, parent.extent.end.offset)

            candidates.append(
                DeclCandidate(
                    start=start_offset,
                    end=end_offset,
                    type_spelling=type_spelling,
                    name=var.spelling,
                    indent=indent,
                    scope_key=scope,
                )
            )
        for child in cursor.get_children():
            visit(child)

    visit(tu.cursor)
    return candidates


def build_replacements(
    source_text: str, candidates: Iterable[DeclCandidate]
) -> List[Replacement]:
    by_scope: Dict[Tuple[int, int], List[DeclCandidate]] = defaultdict(list)
    for cand in candidates:
        by_scope[cand.scope_key].append(cand)

    replacements: List[Replacement] = []

    for scope_cands in by_scope.values():
        ordered = sorted(scope_cands, key=lambda c: c.start)
        i = 0
        while i < len(ordered):
            j = i + 1
            while (
                j < len(ordered)
                and ordered[j].type_spelling == ordered[i].type_spelling
                and source_text[ordered[j - 1].end : ordered[j].start].strip() == ""
                and ordered[j].indent == ordered[i].indent
            ):
                j += 1
            if j - i >= 2:
                group = ordered[i:j]
                start = group[0].start
                end = group[-1].end
                indent = group[0].indent
                type_spelling = group[0].type_spelling
                names = ", ".join(c.name for c in group)

                original_block = source_text[start:end]
                line_end = "\n" if original_block.endswith("\n") else ""
                replacement_text = f"{indent}{type_spelling} {names};{line_end}"

                replacements.append(
                    Replacement(start=start, end=end, text=replacement_text)
                )
                i = j
            else:
                i += 1

    return sorted(replacements, key=lambda r: r.start)


def apply_replacements(source_text: str, replacements: List[Replacement]) -> str:
    if not replacements:
        return source_text

    pieces: List[str] = []
    cursor = 0
    for repl in replacements:
        if repl.start < cursor:
            raise SystemExit("Overlapping replacements detected; aborting.")
        pieces.append(source_text[cursor:repl.start])
        pieces.append(repl.text)
        cursor = repl.end
    pieces.append(source_text[cursor:])
    return "".join(pieces)


def process_file(
    source_path: Path,
    clang_args: Sequence[str],
    libclang_path: str | None,
    dry_run: bool,
) -> bool:
    configure_libclang(libclang_path)
    tu = parse_tu(source_path, clang_args)
    source_text = source_path.read_text(encoding="utf-8")
    candidates = gather_decl_candidates(tu, source_path, source_text)
    replacements = build_replacements(source_text, candidates)
    if not replacements:
        return False
    new_text = apply_replacements(source_text, replacements)
    if dry_run:
        print(f"Would rewrite {source_path} ({len(replacements)} merged block(s)).")
        return True
    source_path.write_text(new_text, encoding="utf-8")
    print(f"Updated {source_path}: merged {len(replacements)} block(s).")
    return True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="Path to the C source file.")
    parser.add_argument(
        "--libclang",
        help="Optional path to libclang shared library.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Report changes without modifying the file.",
    )
    parser.add_argument(
        "--clang-arg",
        action="append",
        default=[],
        help="Additional argument passed to libclang (can be repeated).",
    )
    args = parser.parse_args()

    clang_args = args.clang_arg or []
    changed = process_file(
        source_path=args.source.resolve(),
        clang_args=clang_args,
        libclang_path=args.libclang,
        dry_run=args.dry_run,
    )
    if not changed:
        print("No declaration blocks eligible for merging.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
