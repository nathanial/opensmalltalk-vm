# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

OpenSmalltalk VM (Cog VM) is the virtual machine for Cuis and Squeak Smalltalk dialects. This repository contains the C/C++ source code for the VM and platform-specific support code. All code in this repository is **manually maintained** - you can and should edit the C sources directly to develop the VM.

This is a **simplified fork** focused on macOS ARM development with a Stack Spur VM only.

## First-Time Setup

**REQUIRED**: After cloning, you MUST run:
```bash
./scripts/updateSCCSVersions
```

This installs git filters, post-commit, and post-merge hooks to update sq*SCCSVersion.h files with revision timestamps. Without this, version stamps will be incorrect in compiled VMs.

## Build Commands

### Building the VM on macOS ARM

Navigate to the build directory and use the `mvm` script:
```bash
cd building/macos64ARMv8/squeak.stack.spur
./mvm -f              # Build production/fast VM only
./mvm -a              # Build assert VM only
./mvm -d              # Build debug VM only
./mvm -A              # Build all three (assert, debug, fast)
```

Output apps are created in the build directory (e.g., `Squeak.app`, `SqueakAssert.app`, `SqueakDebug.app`). Build logs are written to `LOGF`, `LOGA`, `LOGD` respectively.

### Top-Level Build Script

For convenience, you can use the top-level build script:
```bash
./build-macos-arm-stack.sh    # Build macOS ARM Stack VM
```

### Other Utility Scripts

```bash
./format.sh           # Format C code (check script for usage)
./mkNamedPrims.sh     # Generate named primitives header
```

## Repository Structure

### VM Source Directory (`src/spur64.stack/`)

The VM implementation for 64-bit Stack interpreter with Spur memory manager:

- `interp.c` - The main interpreter file (includes all other .c files)
- `interp.h` - VM configuration defines
- `validImage.c` - Image validation code
- `vmCallback.h` - VM callback structures
- `update_includes.sh` - Updates include paths in interp.c

The `interp/` subdirectory contains the modularized interpreter code:
- `interp/execution/` - Bytecode execution and method dispatch
- `interp/gc/` - Garbage collection (Spur memory manager)
- `interp/objects/` - Object representation and manipulation
- `interp/primitives/` - Primitive operations
- `interp/stack/` - Stack management
- `interp/processes/` - Process scheduling
- `interp/ffi/` - Foreign function interface
- `interp/image/` - Image loading/saving
- `interp/profiling/` - VM profiling support
- `interp/utils/` - Utility functions

**Key Architecture**: The `interp.c` file includes all the .c files from the `interp/` subdirectories. When you edit files in `interp/*/`, you're editing code that gets included into the main interpreter. Use `update_includes.sh` to update include paths if you move files.

### Plugins (`src/plugins/`)

All plugins are under `src/plugins/`. Each plugin is in its own directory with platform-specific subdirectories (e.g., `B2DPlugin/`, `BitBltPlugin/`, etc.).

The build directory specifies which plugins to include:
- `building/macos64ARMv8/squeak.stack.spur/plugins.int` - Internal plugins (linked into VM)
- `building/macos64ARMv8/squeak.stack.spur/plugins.ext` - External plugins (separate bundles)

To modify which plugins are built, edit these files and run `./mvm -f`.

Common plugins include:
- `B2DPlugin` - 2D graphics (Balloon 2D)
- `BitBltPlugin` - BitBlt operations
- `ZipPlugin` - Zip compression
- `FilePlugin` - File operations
- `SocketPlugin` - Network sockets
- `SoundPlugin` - Sound I/O
- `FloatArrayPlugin`, `Float64ArrayPlugin` - Array primitives
- `SqueakFFIPrims` - Foreign function interface

### Platform Support (`platform/`)

Platform-specific code that combines with VM sources:
- `platform/vm/` - Cross-platform VM support code
- `platform/OSX/` - macOS-specific code (Cocoa, Metal, etc.)
- `platform/Common/` - Common macOS support code
- `platform/Common/Classes/` - macOS Objective-C classes
- `platform/third-party/` - Third-party libraries (fdlibm math library)
- `platform/util/` - Build utility scripts

### Build System (`building/macos64ARMv8/`)

Build configuration for macOS ARM:
- `squeak.stack.spur/` - The only VM build configuration
  - `mvm` - Build script
  - `Makefile` - Points to VM source and includes common makefiles
  - `plugins.int` - Internal plugins list
  - `plugins.ext` - External plugins list
  - `build/` - Build artifacts
  - `deps/` - Dependency files
  - `Squeak.app` - Built VM application

- `common/` - Shared makefiles for macOS builds:
  - `Makefile.app` - Builds and signs the .app bundle
  - `Makefile.app.squeak` - Squeak-specific app configuration
  - `Makefile.vm` - Builds the VM executable and plugins
  - `Makefile.plugin` - Template for building individual plugins
  - `Makefile.flags` - Compilation flags and SDK configuration
  - `Makefile.rules` - Compilation rules for C/C++/Objective-C

- `gdbarm32/`, `gdbarm64/` - ARM processor simulator support libraries
  - Run `./conf.COG` then `./makeem` to build these if doing JIT development

- `HowToBuild` - Detailed platform-specific build instructions

### Image Directory (`image/`)

Scripts to build Smalltalk images for testing:
- `buildspurtrunkvmmaker64image.sh` - Build 64-bit VMMaker image
- `buildspurtrunkvmmakerimage.sh` - Build 32-bit VMMaker image
- `buildspurtrunkreader64image.sh` - Build 64-bit test/REPL image
- `getGoodSpur64VM.sh` - Download a working VM for bootstrapping

These are primarily for testing the VM with Smalltalk images.

### Scripts Directory (`scripts/`)

Utility scripts:
- `updateSCCSVersions` - Install version hooks (**MUST run after clone**)
- `gitci`, `gitciplugins` - Commit helpers for source tree organization
- Python scripts for C code refactoring:
  - `uninline_*.py` - Un-inline specific functions
  - `remove_*.py` - Remove macros and patterns
  - `compact_*.py` - Compact declarations
  - `merge_local_decls.py` - Merge local variable declarations
  - `refactor_goto_chains.py` - Refactor goto chains
  - `recompose_interp_with_includes.py` - Recompose interpreter with includes

## Development Workflow

### Editing VM Source Code

The VM code is manually maintained and can be edited directly:

1. **Core interpreter code**: Edit files in `src/spur64.stack/interp/*/`
   - The interpreter is modularized by function across subdirectories
   - `interp.c` includes all these files
   - If you move files, run `src/spur64.stack/update_includes.sh`

2. **Plugins**: Edit files in `src/plugins/PluginName/`
   - Each plugin has its own directory
   - Platform-specific code goes in subdirectories (e.g., `macos/`, `unix/`)

3. **Platform code**: Edit files in `platform/OSX/`, `platform/Common/`, or `platform/vm/`

4. **Rebuild**: After editing, rebuild the VM:
   ```bash
   cd building/macos64ARMv8/squeak.stack.spur
   ./mvm -f
   ```

### VM Build Configurations

- **Production**: Fully optimized for production use (`./mvm -f`)
- **Assert**: Optimized with -O1, includes assert checks for debugging (`./mvm -a`)
- **Debug**: No optimization (-O0), maximum debuggability (`./mvm -d`)

### Adding or Removing Plugins

1. Edit `building/macos64ARMv8/squeak.stack.spur/plugins.int` (internal) or `plugins.ext` (external)
2. Add or remove the plugin name from the list
3. Run `./mvm -f` to rebuild

### Git Integration

The repository uses source tree substitutions for `sq*SCCSVersion.h` files, replacing `$Rev$`, `$Date$`, `$URL$`, `$CommitHash$` with actual values. This is why `./scripts/updateSCCSVersions` is required after cloning.

If you use git reset or checkout, rerun `./scripts/updateSCCSVersions` to ensure stamps are correct.

### Commit Workflow

- Create feature branches from `Cog` branch
- Make small, logical commits that still compile
- Avoid rebasing/squashing - keep history
- Use `[ci skip]` in commit message for documentation-only changes

## Code Style (C Sources)

When editing existing files, follow these conventions:

### Function Declarations
```c
static int
convertCopy(char *from, int fromLen, char *to, int toLen, int term)
```
Type on one line, name and parameters on the next (enables searching for definitions by name at start of line).

### Reserved Words
Use `if (expr)` NOT `if(expr)`. Apply to `do`, `for`, `while`.

### Complex Conditionals
```c
if (expr1
 || expr2)
```
Put `||` and `&&` at beginning of lines, not end (more visible).

### Indentation
4-space tabs. When changing semantic code, preserve existing formatting to avoid confusing git history.

## Testing and Debugging

### Running Tests
Build a test Smalltalk image and run it with your compiled VM. See `image/` directory scripts for building test images.

Example:
```bash
# Build or download a test image
cd image
./getGoodSpur64VM.sh

# Run your VM with the image
../building/macos64ARMv8/squeak.stack.spur/Squeak.app/Contents/MacOS/Squeak <path-to-image>
```

### Debugging with lldb

Debug and assert builds include full symbols:
```bash
lldb building/macos64ARMv8/squeak.stack.spur/SqueakAssert.app/Contents/MacOS/Squeak
```

Many debugging functions exist in VM source (e.g., functions starting with `print` in `interp.c` and various `interp/` subdirectories).

For headless builds via SSH:
```bash
# Unlock keychain for code signing
security -v unlock-keychain [-p <PASSWORD>] /Users/$USER/Library/Keychains/login.keychain

# Enable debugging
/usr/sbin/DevToolsSecurity --enable
```

## Platform-Specific Notes

### macOS Requirements

- macOS 10.13+ with Xcode 10.1+ (for Metal graphics support)
- Clang 10+
- Set `SIGNING_IDENTITY` environment variable for code signing (optional but recommended):
  ```bash
  export SIGNING_IDENTITY="Developer ID Application: Your Name"
  ```

### Processor Simulators (for JIT Development)

To develop the JIT, build ARM processor simulator support libraries:
```bash
cd building/macos64ARMv8/gdbarm64
./conf.COG
./makeem
```

The VM will include the simulator plugins when these libraries are built.

## Memory Manager: Spur

This VM uses the **Spur** memory manager, which features:
- Generation scavenging garbage collection
- Lazy forwarding for fast `become:` operations
- Unified object header format for 32-bit and 64-bit
- Segmented heap that can grow and shrink

Spur is used by Squeak 5+, Cuis 5+, and is significantly faster than the old v3 memory manager.

## Additional Resources

- Spur memory manager paper: "A partial read barrier for efficient support of live object-oriented programming" by Miranda & Béra
- Community forums:
  - Cuis: https://cuis.st/community
  - Squeak: https://squeak.org/community
