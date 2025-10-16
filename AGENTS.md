# Repository Guidelines

## Project Structure & Module Organization
- `src/spur64.stack` — core VM sources for the Stack Spur variant.
- `src/plugins/<PluginName>/` — built‑in plugins (e.g., `ZipPlugin/ZipPlugin.cpp`).
- `platforms/` — platform code (Cross, macOS, unix) and headers.
- `building/macos64ARMv8/squeak.stack.spur/` — macOS ARM build directory (Makefiles, `mvm`).
- `scripts/`, `deploy/`, `include/`, `image/` — tooling, assets, and headers.
- `tests/` — smoke tests or artifacts (if present). Avoid committing binaries.

## Build, Test, and Development Commands
- Full macOS ARM build (product + app bundle):
  - `./build-macos-arm-stack.sh`
- Build VM only (from build dir):
  - `cd building/macos64ARMv8/squeak.stack.spur && ./mvm -f`
- Build a single plugin (example: ZipPlugin):
  - `cd building/macos64ARMv8/squeak.stack.spur && make -j2 build/vm/ZipPlugin.lib`
- Run locally:
  - App: `open building/.../Squeak.app`
  - CLI: `building/.../Squeak.app/Contents/MacOS/Squeak <path-to-image>`

## Coding Style & Naming Conventions
- Match surrounding file style; do not mass‑reformat.
- C/C++/ObjC: keep includes ordered (config → platform → VM headers), prefer explicit casts from `void*`.
- Plugin layout: `src/plugins/<Name>/<Name>.c(pp)` with exported primitives via `EXPORT(...)` and VM proxy access through `setInterpreter`.
- Avoid C++ keywords as identifiers (e.g., use `klass` instead of `class`).

## Testing Guidelines
- No formal unit test harness. Perform smoke tests by launching the VM with a known image and exercising the affected primitives.
- Keep changes small; include simple reproduction steps in PRs.

## Commit & Pull Request Guidelines
- Commits: concise, imperative subject (≤72 chars), body explains “why” and build impact. Group related changes only.
- PRs: include platform, build commands used, and a brief test plan. Link issues and attach relevant logs (e.g., `LOGF` from the build dir).
- Update docs when altering public APIs, build flags, or plugin wiring (`plugins.int`/`plugins.ext`).

## Agent‑Specific Instructions
- Use `apply_patch` to edit files; read large files in ≤250‑line chunks.
- Prefer minimal, surgical diffs; avoid unrelated fixes and whitespace churn.
- After renaming sources (e.g., `.c` → `.cpp`), clean stale deps under `building/.../deps/` if needed and ensure Makefile patterns cover the new extension.

