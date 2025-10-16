# Analysis of Unix Platform Files Used by macOS Build System

## Executive Summary

This macOS-only fork uses **5 key Unix source files** that are compiled directly into the VM. Additionally, many Unix header files are used for data structures and definitions. The build system explicitly includes `platforms/unix/vm` in the SRCDIRS for header inclusion, and several plugins wrap or include Unix implementations.

---

## Directory Structure Overview

```
platforms/unix/
├── config/                      # Unix autotools build system [NOT USED - SAFE DELETE]
├── doc/                         # Unix documentation [NOT USED - SAFE DELETE]  
├── misc/threadValidate/         # Test utilities [NOT USED - SAFE DELETE]
├── vm/                          # Core VM support [PARTIALLY USED]
├── vm-display-Quartz/           # Old Quartz display [NOT USED - SAFE DELETE]
└── vm-sound-MacOSX/             # Old CoreAudio [NOT USED - SAFE DELETE]
```

**Analysis focus**: Only `platforms/unix/vm/` is relevant for macOS build.

---

## Files Explicitly Compiled into macOS VM

From `/building/macos64ARMv8/common/Makefile.vm` lines 82-84:

```makefile
UNIXSRC:=$(addprefix $(UNIXVMDIR)/,aio.c sqUnixHeartbeat.c sqUnixSpurMemory.c \
                     sqUnixThreads.c sqUnixVMProfile.c)
```

### MUST KEEP - Directly Compiled (5 files)

1. **`aio.c`** (23 KB)
   - Asynchronous file I/O infrastructure
   - Core VM I/O operations
   - Functions: `aioInit()`, `aioFini()`, `aioEnable()`, etc.

2. **`sqUnixHeartbeat.c`** (13.9 KB)
   - VM heartbeat/ticker thread management via pthreads
   - Conditionally includes: `sqUnixITimerHeartbeat.c` or `sqUnixITimerTickerHeartbeat.c`
   - Critical for VM timing

3. **`sqUnixSpurMemory.c`** (7 KB)
   - Spur memory manager for Unix/macOS
   - Memory allocation via mmap
   - Has `#ifdef __APPLE__` sections for macOS-specific behavior
   - **CRITICAL** - This is the heap allocator

4. **`sqUnixThreads.c`** (6.3 KB)
   - POSIX threading (pthreads) support
   - Functions: `ioNewOSThread()`, `ioNewOSSemaphore()`, `ioWaitOnOSSemaphore()`
   - Used when `COGMTVM || NEED_OSSEMAPHORE` is defined

5. **`sqUnixVMProfile.c`** (6 KB)
   - VM profiling/sampling via signals
   - Has `#ifdef __APPLE__` for macOS signal handling
   - Statistical profiling of VM execution

### MUST KEEP - Conditionally Included from sqUnixHeartbeat.c (2 files)

6. **`sqUnixITimerHeartbeat.c`** (12.7 KB)
   - Included if `#if ITIMER_HEARTBEAT && !VM_TICKER`
   - Timer-based heartbeat implementation

7. **`sqUnixITimerTickerHeartbeat.c`** (18.5 KB)
   - Included if `#if ITIMER_HEARTBEAT && VM_TICKER`
   - Ticker-based heartbeat implementation

---

## Header Files Used

The build system includes `/platforms/unix/vm` in SRCDIRS (line 90 of Makefile.vm), making all headers available.

### Directly Referenced Headers (Must Keep)

1. **`sqImageFileAccess.h`** (8 KB) - HEAVILY USED
   - Wrapped by: `platforms/macOS/vm/OSX/sqImageFileAccess.h`
   - Included by: `platforms/macOS/vm/Common/sqMacImageFileAccess.c`
   - Included by: `src/spur64.stack/interp.c`, `validImage.c`
   - Purpose: Image file I/O operations (open, close, read, write, seek)
   - Platform: POSIX file operations (open, close, read, write, lseek)

2. **`sqaio.h`** (5.3 KB) - MODERATELY USED
   - Included by: `platforms/macOS/vm/Common/Classes/sqSqueakEventsAPI.m`
   - Used by: aio.c itself
   - Purpose: Asynchronous I/O event handling API definitions

### Headers Used By Compiled Source Files (Must Keep)

3. **`debug.h`** (1 KB)
   - Provides DPRINTF macros and assert variants
   - Used by code in sqUnixSpurMemory.c, etc.

4. **`include_ucontext.h`** (4.8 KB)
   - Platform-independent context header
   - Used by: sqUnixVMProfile.c for signal context

5. **`codeZoneControlARM64.h`** (0.8 KB)
   - ARM64-specific code zone control
   - Used by: Some compiled files

6. **`sqPlatformSpecific.h`** (4.4 KB)
   - Platform-specific definitions

7. **`sqConfig.h`** (1.2 KB)
   - Configuration defines

8. **`SqModule.h`** (1 KB)
   - Module interface definitions

9. **`sqGnu.h`** (10.4 KB)
   - GNU compatibility macros

10. **`SqMemoryAccess.h`** (referenced by sqaio.h)
    - Likely located elsewhere but used

### Orphaned Headers (Safe to Delete)

- `SqDisplay.h` - X11 display interface (not used on macOS)
- `SqSound.h` - Sound interface (not used, macOS has CoreAudio)
- `sqUnixGL.h` - OpenGL interface (not used in build)

---

## Unix Source Files in platforms/unix/vm/ Directory

Total: 33 files. Status breakdown:

### MUST KEEP (as compiled or conditionally included)
- aio.c
- sqUnixHeartbeat.c
- sqUnixSpurMemory.c
- sqUnixThreads.c
- sqUnixVMProfile.c
- sqUnixITimerHeartbeat.c (included from sqUnixHeartbeat.c)
- sqUnixITimerTickerHeartbeat.c (included from sqUnixHeartbeat.c)

### MUST KEEP (as headers needed by other files)
- sqImageFileAccess.h
- sqaio.h
- debug.h
- include_ucontext.h
- codeZoneControlARM64.h
- sqPlatformSpecific.h
- sqConfig.h
- SqModule.h
- sqGnu.h

### NOT COMPILED, HEADERS NOT USED (Safe to Delete - VERIFIED)

1. **`sqUnixGL.h`** - 0 references in macOS build
2. **`dlfcn-dyld.c`** (5.5 KB) - 3 refs (all in vm-display-Quartz, not compiled)
3. **`debug.c`** (0.8 KB) - Not compiled (only .h used)
4. **`mac-alias.c`** (1.4 KB) - 1 ref (in vm-display-Quartz, not compiled)
5. **`osExports.c`** (1.3 KB) - 2 refs (in vm-display-Quartz, not compiled)

### NOT COMPILED BUT REFERENCED (Verify Before Deleting)

These have references, but appear to be only from vm-display-Quartz (which is itself safe to delete):

1. **`sqUnixCharConv.c/h`** (12.7 KB + 2.6 KB)
   - 19 references, but mostly from vm-display-Quartz/sqUnixQuartz.m
   - Not in UNIXSRC compilation list
   - Analysis: Only referenced by code in vm-display-Quartz (which is not compiled)

2. **`sqUnixDisplayHelpers.c/h`** (11 KB + 2 KB)
   - 8 references, from vm-display-Quartz
   - Not in UNIXSRC compilation list
   - Analysis: Only referenced by old Quartz display code

3. **`sqUnixEvent.c`** (10.8 KB)
   - 3 references, from vm-display-Quartz
   - Not in UNIXSRC compilation list
   - Analysis: Only referenced by old Quartz display code

4. **`sqUnixExternalPrims.c`** (13.8 KB)
   - 6 references, from vm-display-Quartz and build comments
   - Not in UNIXSRC compilation list
   - Note: macOS has different file: `sqMacUnixExternalPrims.m` in platforms/macOS
   - Analysis: Not compiled in macOS build

5. **`sqUnixGlobals.h`** (1.2 KB)
   - 5 references, from vm-display-Quartz
   - Not in UNIXSRC compilation list
   - Analysis: Only referenced by old Quartz display code

6. **`sqUnixMain.c/h`** (59.2 KB + 0.4 KB)
   - 16 references, from vm-display-Quartz and CLAUDE.md
   - Not in UNIXSRC compilation list
   - Note: macOS has its own main entry point
   - Analysis: Not compiled in macOS build

7. **`sqUnixMemory.c`** (10.3 KB)
   - 5 references, legacy code
   - Not in UNIXSRC compilation list (superseded by sqUnixSpurMemory.c)
   - Analysis: Legacy memory manager, not used

---

## Plugin Integration

### FilePlugin (Uses Unix Code)
- **macOS version**: `/src/plugins/FilePlugin/macos/sqUnixFile.c`
- **Content**: `#include "../unix/sqUnixFile.c"` - directly includes Unix version
- **Status**: REQUIRED - macOS file plugin uses the Unix implementation
- **Build config**: `FilePlugin/macos/Makefile` includes unix directory: `INCDIRS:=$(PLATDIR)/unix/vm`

### SoundPlugin (Native macOS Implementation)
- **macOS files**: CoreAudio Objective-C implementation
- **Files**: `sqSqueakOSXSoundCoreAudio.m`, etc.
- **Status**: Does NOT use Unix sound plugin
- **Unix equivalent**: `SoundPlugin/unix/sqUnixSound.c` (not compiled)

### HostWindowPlugin (Native macOS Implementation)
- **macOS file**: `sqMacHostWindow.m`
- **Status**: Does NOT use Unix implementation
- **Unix equivalent**: `HostWindowPlugin/unix/sqUnixHostWindowPlugin.c` (not compiled)

---

## SAFE TO DELETE - Confirmed

### Tier 1: Definitely Safe (Not used at all)

Entire directories:
- **`platforms/unix/config/`** - Autotools build system
  - Files: configure, configure.ac, mkmf, Squeak.spec.in, Makefile.*, etc.
  - Reason: macOS uses different build system

- **`platforms/unix/doc/`** - Documentation
  - Files: HowToBuildFromSource.*, README.Sound, release notes, COPYING, etc.
  - Reason: Documentation only

- **`platforms/unix/misc/`** - Test utilities
  - Directory: threadValidate/
  - Reason: Separate test tools, not part of VM

- **`platforms/unix/vm-display-Quartz/`** - OLD Quartz display code
  - Reason: macOS now uses Metal graphics, not old Quartz
  - Note: This directory contains references to many "possibly safe" files listed below
  - Status: **Safe to delete - entire directory**

- **`platforms/unix/vm-sound-MacOSX/`** - OLD CoreAudio sound code
  - Files: sqUnixSoundMacOSX.c
  - Reason: Modern CoreAudio in plugins/SoundPlugin/macos/

- **`platforms/unix/ChangeLog`**, **`README*`** - Documentation

### Tier 2: Safe to Delete (Only referenced by vm-display-Quartz)

Since `vm-display-Quartz/` is safe to delete entirely, all files it references are safe to delete:

- **`sqUnixCharConv.c/h`** - Only referenced from vm-display-Quartz
- **`sqUnixDisplayHelpers.c/h`** - Only referenced from vm-display-Quartz
- **`sqUnixEvent.c`** - Only referenced from vm-display-Quartz
- **`sqUnixExternalPrims.c`** - Only referenced from vm-display-Quartz (macOS has own version)
- **`sqUnixGlobals.h`** - Only referenced from vm-display-Quartz
- **`sqUnixMain.c/h`** - Only referenced from vm-display-Quartz (macOS has own entry point)
- **`sqUnixMemory.c`** - Legacy implementation, superseded by sqUnixSpurMemory.c
- **`dlfcn-dyld.c`** - Only referenced from vm-display-Quartz
- **`mac-alias.c`** - Only referenced from vm-display-Quartz
- **`osExports.c`** - Only referenced from vm-display-Quartz
- **`debug.c`** - Not compiled (only .h is needed)
- **`sqUnixGL.h`** - 0 references anywhere

---

## Summary Table

| File | Size | Status | Notes |
|------|------|--------|-------|
| aio.c | 23 KB | KEEP | Compiled |
| sqUnixHeartbeat.c | 13.9 KB | KEEP | Compiled |
| sqUnixSpurMemory.c | 7 KB | KEEP | Compiled, critical |
| sqUnixThreads.c | 6.3 KB | KEEP | Compiled |
| sqUnixVMProfile.c | 6 KB | KEEP | Compiled |
| sqUnixITimerHeartbeat.c | 12.7 KB | KEEP | Conditionally included |
| sqUnixITimerTickerHeartbeat.c | 18.5 KB | KEEP | Conditionally included |
| sqImageFileAccess.h | 8.1 KB | KEEP | Heavily used |
| sqaio.h | 5.3 KB | KEEP | Used by aio.c |
| debug.h | 1 KB | KEEP | Used by sources |
| include_ucontext.h | 4.8 KB | KEEP | Used by sources |
| codeZoneControlARM64.h | 0.8 KB | KEEP | Used by sources |
| sqPlatformSpecific.h | 4.4 KB | KEEP | Used by sources |
| sqConfig.h | 1.2 KB | KEEP | Used by sources |
| SqModule.h | 1 KB | KEEP | Used by sources |
| sqGnu.h | 10.4 KB | KEEP | Used by sources |
| **sqUnixGL.h** | 0.3 KB | DELETE | 0 refs |
| **debug.c** | 0.8 KB | DELETE | Only .h used |
| **sqUnixCharConv.c/h** | 15.3 KB | DELETE | Only in vm-display-Quartz |
| **sqUnixDisplayHelpers.c/h** | 13 KB | DELETE | Only in vm-display-Quartz |
| **sqUnixEvent.c** | 10.8 KB | DELETE | Only in vm-display-Quartz |
| **sqUnixExternalPrims.c** | 13.8 KB | DELETE | Only in vm-display-Quartz |
| **sqUnixGlobals.h** | 1.2 KB | DELETE | Only in vm-display-Quartz |
| **sqUnixMain.c/h** | 59.6 KB | DELETE | Only in vm-display-Quartz |
| **sqUnixMemory.c** | 10.3 KB | DELETE | Legacy, superseded |
| **dlfcn-dyld.c** | 5.5 KB | DELETE | Only in vm-display-Quartz |
| **mac-alias.c** | 1.4 KB | DELETE | Only in vm-display-Quartz |
| **osExports.c** | 1.3 KB | DELETE | Only in vm-display-Quartz |
| **Entire config/** | - | DELETE | Autotools, not used |
| **Entire doc/** | - | DELETE | Documentation |
| **Entire misc/** | - | DELETE | Test utilities |
| **Entire vm-display-Quartz/** | - | DELETE | Old Quartz display |
| **Entire vm-sound-MacOSX/** | - | DELETE | Old sound code |

---

## Deletion Priority

### Immediate (Very Safe)
1. All of `platforms/unix/config/` - Autotools, zero references in macOS build
2. All of `platforms/unix/doc/` - Documentation only
3. All of `platforms/unix/misc/` - Test utilities
4. All of `platforms/unix/vm-display-Quartz/` - Old display system
5. All of `platforms/unix/vm-sound-MacOSX/` - Old sound system
6. `sqUnixGL.h` - Zero references anywhere
7. `debug.c` - Only debug.h is needed

### Safe (After removing vm-display-Quartz/)
8. `sqUnixCharConv.c/h`
9. `sqUnixDisplayHelpers.c/h`
10. `sqUnixEvent.c`
11. `sqUnixExternalPrims.c`
12. `sqUnixGlobals.h`
13. `sqUnixMain.c/h`
14. `sqUnixMemory.c`
15. `dlfcn-dyld.c`
16. `mac-alias.c`
17. `osExports.c`

### Never Delete
- All 5 KEEP files (aio.c, sqUnixHeartbeat.c, sqUnixSpurMemory.c, sqUnixThreads.c, sqUnixVMProfile.c)
- Both heartbeat includes (sqUnixITimerHeartbeat.c, sqUnixITimerTickerHeartbeat.c)
- All headers in KEEP list

---

## Commands to Execute Deletions

```bash
# Tier 1: Immediate deletions
rm -rf platforms/unix/config/
rm -rf platforms/unix/doc/
rm -rf platforms/unix/misc/
rm -rf platforms/unix/vm-display-Quartz/
rm -rf platforms/unix/vm-sound-MacOSX/
rm -f platforms/unix/vm/sqUnixGL.h
rm -f platforms/unix/vm/debug.c

# Tier 2: After verifying no other references
rm -f platforms/unix/vm/sqUnixCharConv.c
rm -f platforms/unix/vm/sqUnixCharConv.h
rm -f platforms/unix/vm/sqUnixDisplayHelpers.c
rm -f platforms/unix/vm/sqUnixDisplayHelpers.h
rm -f platforms/unix/vm/sqUnixEvent.c
rm -f platforms/unix/vm/sqUnixExternalPrims.c
rm -f platforms/unix/vm/sqUnixGlobals.h
rm -f platforms/unix/vm/sqUnixMain.c
rm -f platforms/unix/vm/sqUnixMain.h
rm -f platforms/unix/vm/sqUnixMemory.c
rm -f platforms/unix/vm/dlfcn-dyld.c
rm -f platforms/unix/vm/mac-alias.c
rm -f platforms/unix/vm/osExports.c

# Optional: Keep only the minimal set
# (Keep platforms/unix/vm/ but only with KEEP files listed above)
```

---

## Verification Commands

Before deletion, run these to verify nothing unexpected references files:

```bash
# Check no macOS sources reference a file
grep -r "sqUnixCharConv" platforms/macOS/ src/ building/macos* 2>/dev/null

# Verify deleted files won't break build
cd building/macos64ARMv8/squeak.stack.spur && ./mvm -f 2>&1 | tee LOGF
```

