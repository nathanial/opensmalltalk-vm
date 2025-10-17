#!/usr/bin/env bash
# Collapse consecutive single-variable declarations in the split interp sources.

set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
target_dir="$root/src/spur64.stack/interp"

if [[ ! -d "$target_dir" ]]; then
  echo "Expected directory not found: $target_dir" >&2
  exit 1
fi

find "$target_dir" -type f -name '*.c' -print0 \
  | xargs -0 -n 1 -P 4 python3 "$root/scripts/compact_local_decls.py"
