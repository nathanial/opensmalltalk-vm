#!/usr/bin/env bash
# Run clang-format across the interpreter sources (or provided paths).
set -euo pipefail

clang_format=${CLANG_FORMAT:-clang-format}

if ! command -v "$clang_format" >/dev/null 2>&1; then
  echo "clang-format not found (looked for '${clang_format}')." >&2
  exit 1
fi

if [[ $# -eq 0 ]]; then
  targets=(src/spur64.stack/interp src/plugins)
else
  targets=("$@")
fi

paths=()
for target in "${targets[@]}"; do
  if [[ -d "$target" ]]; then
    while IFS= read -r -d '' file; do
      paths+=("$file")
    done < <(find "$target" -type f \( -name '*.c' -o -name '*.h' -o -name '*.hh' -o -name '*.hpp' -o -name '*.cc' -o -name '*.cpp' \) -print0)
  elif [[ -f "$target" ]]; then
    paths+=("$target")
  fi
done

if [[ ${#paths[@]} -eq 0 ]]; then
  echo "No source files found for formatting." >&2
  exit 0
fi

"$clang_format" -i "${paths[@]}"
