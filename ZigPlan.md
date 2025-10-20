# Zig Integration Plan for OpenSmalltalk VM

## Executive Summary

This document outlines a phased approach to gradually integrating Zig into the OpenSmalltalk VM codebase. Since Slang is no longer being used, **all C code is now maintained by hand** and eligible for replacement. The strategy prioritizes low-risk, high-value targets first (utilities and plugins), then progresses to core VM components (interpreter, GC, JIT), while maintaining backward compatibility throughout the migration.

### 🎯 What's Different Without Slang?

**Previous assumption:** Core VM files (`src/spur64.*/interp.c`, `cointerp.c`, etc.) were auto-generated from Smalltalk and off-limits.

**Current reality:** All C code is hand-maintained, meaning:
- ✅ Can replace interpreter bytecode handlers with Zig
- ✅ Can rewrite Spur GC in Zig
- ✅ Can modernize JIT compiler with Zig
- ✅ Entire VM is eligible for gradual Zig migration

This plan now includes **Phase 7** for core VM replacement (~193k+ lines of interpreter code).

## Why Zig?

1. **Seamless C interop** - Direct `@cImport()` of existing headers without FFI wrappers
2. **Incremental adoption** - Zig can compile existing C code, allowing file-by-file migration
3. **Explicit control** - No hidden allocations or control flow, critical for VM development
4. **Cross-compilation** - Built-in cross-compiler for all platforms (macOS, Linux, Windows, ARM)
5. **Modern tooling** - Better build system than make/autoconf while maintaining simplicity

## Critical Constraints

**ALL C CODE IS NOW HAND-MAINTAINED:**
Since Slang is no longer in use, the entire C codebase (including interpreter, GC, JIT, plugins, and platform code) is now hand-written and can be replaced with Zig. However, we recommend a gradual, risk-managed approach:

**Low-Risk Targets (Start Here):**
- Platform utilities (`platforms/Cross/vm/*.c`)
- Simple plugins (`src/plugins/*/`)
- Build scripts and configuration
- Support libraries

**Medium-Risk Targets (After gaining experience):**
- Platform-specific implementations (`platforms/unix/`, `platforms/win32/`)
- Complex plugins (FFI, JIT support)
- VM interface layer

**High-Risk Targets (Core VM - Later phases):**
- Interpreter (`src/spur64.stack/interp.c`, `src/spur64.cog/cointerp.c`)
- Memory manager/GC (`src/spur64.*/validImage.c`, GC routines)
- JIT compiler (`src/spur64.cog/cogit*.c`)
- Bytecode dispatch loops

---

## Phase 1: Foundation (Weeks 1-2)

### Goals
- Set up Zig build infrastructure alongside existing make system
- Prove C/Zig interop works with actual VM headers
- Zero changes to production build

### Tasks

#### 1.1 Install Zig and Create Build Configuration
```bash
# Install Zig 0.13.0 or later
curl https://ziglang.org/download/0.13.0/zig-macos-aarch64-0.13.0.tar.xz | tar xJ

# Create build.zig at project root
```

#### 1.2 Create Proof of Concept
**Target:** Create a simple Zig utility that imports VM headers

**File:** `zig/poc/vm_headers_test.zig`
```zig
const c = @cImport({
    @cInclude("sq.h");
    @cInclude("sqVirtualMachine.h");
});

pub fn main() void {
    // Verify we can access VM constants
    const version = c.VM_PROXY_MAJOR;
    // Simple header parsing test
}
```

#### 1.3 Build Verification
- Ensure Zig can compile with existing C headers
- Test on macOS ARM64 first (current platform)
- Document any header compatibility issues

**Success Criteria:**
- [ ] Zig toolchain installed
- [ ] Can `@cImport()` all major VM headers without errors
- [ ] Basic build.zig compiles test program
- [ ] No disruption to existing build system

---

## Phase 2: Platform Utilities (Weeks 3-5)

### Goals
- Replace hand-written C utilities with Zig equivalents
- Gain experience with VM codebase
- Demonstrate tangible benefits (better error handling, clearer code)

### Targets (in order)

#### 2.1 Text Encoding Utilities
**File:** `platforms/Cross/vm/sqTextEncoding.c` (336 lines, hand-written)
- Character set conversion
- UTF-8 handling
- Self-contained, well-defined interface

**Replacement:** `zig/vm/text_encoding.zig`
- Use Zig's native Unicode support
- Expose C-compatible API via `export` keyword
- Link as static library

#### 2.2 Path Manipulation
**File:** `platforms/Cross/vm/sqPath.c` (hand-written)
- Path joining, normalization
- Platform-independent path operations

**Replacement:** `zig/vm/path.zig`
- Leverage `std.fs.path`
- Simpler, safer implementation

#### 2.3 Named Primitives Registry
**File:** `platforms/Cross/vm/sqNamedPrims.c`
- Plugin name registration
- Function pointer table management

**Replacement:** `zig/vm/named_prims.zig`
- Type-safe function registry
- Better error reporting

**Integration Strategy:**
```c
// In existing C code, add conditional compilation:
#ifdef USE_ZIG_TEXT_ENCODING
  #include "zig/vm/text_encoding.h"
#else
  // existing implementation
#endif
```

**Success Criteria:**
- [ ] All three utilities ported to Zig
- [ ] Pass existing test suites (if any)
- [ ] Benchmarks show no performance regression
- [ ] Can toggle between C/Zig implementations via build flag

---

## Phase 3: Simple Plugin Replacement (Weeks 6-9)

### Goals
- Replace an entire plugin with Zig
- Establish plugin development patterns
- Validate external plugin interface

### Target Plugins (choose one to start)

#### Option A: UUIDPlugin (Simplest)
**Current:** `src/plugins/UUIDPlugin/UUIDPlugin.c`
- ~300 lines
- Simple UUID generation
- Minimal dependencies
- Good starter project

#### Option B: MD5Plugin
**Current:** `src/plugins/MD5Plugin/MD5Plugin.c`
- Well-defined algorithm
- Easy to verify correctness
- Self-contained

#### Option C: FileAttributesPlugin (Most Useful)
**Current:** `src/plugins/FileAttributesPlugin/FileAttributesPlugin.c`
- File metadata access
- Zig's `std.fs` is superior to C equivalents
- High impact on code clarity

**Recommended:** Start with UUIDPlugin

### Implementation

**File Structure:**
```
src/plugins/UUIDPlugin/
├── UUIDPlugin.c           (original, keep for now)
├── UUIDPlugin.zig         (new implementation)
├── build.zig              (plugin build config)
└── plugin_interface.zig   (Smalltalk primitive interface)
```

**Plugin Interface Pattern:**
```zig
// plugin_interface.zig
const c = @cImport({
    @cInclude("sqVirtualMachine.h");
});

export fn setInterpreter(anInterpreter: *c.VirtualMachine) c.sqInt {
    // Plugin initialization
    return 1; // success
}

export fn primitiveUUIDGenerate() void {
    // Smalltalk primitive implementation
    // Return UUID to Smalltalk stack
}
```

**Build Integration:**
- Add to `plugins.int` or `plugins.ext` conditionally
- Link Zig-compiled `.o` file alongside C plugins
- No changes to VM core

**Success Criteria:**
- [ ] Plugin compiles and links with VM
- [ ] Smalltalk can call primitives
- [ ] All existing UUID tests pass
- [ ] Plugin loads on macOS ARM64
- [ ] Document plugin development guide

---

## Phase 4: Platform Layer Components (Weeks 10-14)

### Goals
- Replace larger platform-specific subsystems
- Improve cross-platform consistency
- Leverage Zig's cross-compilation

### Targets

#### 4.1 Ticker/Timing System
**Files:**
- `platforms/Cross/vm/sqTicker.c`
- Platform-specific timer implementations

**Replacement:** `zig/platform/ticker.zig`
- Unified cross-platform timing
- Better precision with Zig's time APIs

#### 4.2 External Semaphores
**File:** `platforms/Cross/vm/sqExternalSemaphores.c`
- OS synchronization primitives
- Event handling

**Replacement:** `zig/platform/semaphores.zig`
- Use `std.Thread.Semaphore` where possible
- Platform-specific fallbacks

#### 4.3 Virtual Machine Interface Glue
**File:** `platforms/Cross/vm/sqVirtualMachine.c`
- VM API implementation
- Plugin communication

**Replacement:** `zig/vm/vm_interface.zig`
- Type-safe API
- Better error handling

**Success Criteria:**
- [ ] Platform layer compiles on macOS ARM64
- [ ] Port to Linux x86_64 to test cross-compilation
- [ ] All existing platform tests pass
- [ ] VM runs Smalltalk image without issues

---

## Phase 5: Build System Integration (Weeks 15-18)

### Goals
- Make Zig a first-class build option
- Simplify cross-compilation
- Reduce build system complexity

### Tasks

#### 5.1 Unified Build System
**Create:** `build.zig` (project root)
- Compile VM core (still C, via Zig's C compiler)
- Compile Zig components
- Link plugins
- Generate platform-specific bundles

#### 5.2 Replace Makefiles (Optional)
**Current:**
- `building/macos64ARMv8/common/Makefile.*`
- Complex autoconf/make setup

**Replacement:**
- Single `build.zig` for all platforms
- `zig build -Dtarget=aarch64-macos` for cross-compilation

**Benefits:**
- One build system to maintain
- Built-in cross-compilation
- Faster builds (Zig caches aggressively)
- No need for separate Linux/Windows/macOS build instructions

#### 5.3 CI Integration
**Update:** `.github/workflows/*.yml`
- Add Zig installation step
- Build with Zig-enabled configuration
- Run alongside existing C builds (parallel validation)

**Success Criteria:**
- [ ] Can build entire VM with `zig build`
- [ ] Cross-compile macOS ARM → Linux x86_64
- [ ] CI builds pass with Zig components
- [ ] Build time improvement documented

---

## Phase 6: Advanced Integration (Weeks 19+)

### Goals
- New features written in Zig by default
- Performance-critical hot paths
- Enhanced debugging/introspection

### Potential Projects

#### 6.1 FFI Call Interface Rewrite
**Current:** `platforms/*/vm/sqFFI*.c`
- Complex, platform-specific
- Error-prone

**Zig Advantages:**
- Better inline assembly
- Safer stack manipulation
- Clearer ABI handling

#### 6.2 Memory Profiling Tools
**New:** `zig/tools/heap_profiler.zig`
- Integration with Spur GC
- Real-time memory analysis
- Better than existing C debugging tools

#### 6.3 JIT Support Code
**Current:** Bochs/GDB integration for simulation
**Potential:** Zig-based lightweight JIT testing framework

---

## Phase 7: Core VM Components (Weeks 25+)

### Goals
- Replace core interpreter/GC/JIT components with Zig
- Achieve significant performance improvements
- Modernize VM architecture

**WARNING:** These are high-risk changes to critical VM components. Only attempt after extensive experience with Phases 1-6.

### Targets (in recommended order)

#### 7.1 Bytecode Interpreter
**Files:**
- `src/spur64.stack/interp.c` (~193k lines)
- `src/spur64.cog/cointerp.c` (JIT variant)

**Strategy:**
- Start with interpreter primitives (individual bytecode handlers)
- Replace one bytecode at a time, with extensive testing
- Use Zig's `switch` with comptime for bytecode dispatch
- Maintain identical semantics to C version

**Example:**
```zig
fn interpretBytecode(bytecode: u8) InterpreterResult {
    return switch (bytecode) {
        0x10 => pushReceiverVariable(0),
        0x11 => pushReceiverVariable(1),
        // ... 256 bytecodes
        else => unknownBytecode(bytecode),
    };
}
```

**Benefits:**
- Clearer bytecode dispatch logic
- Better bounds checking
- Easier to add instrumentation

#### 7.2 Object Memory & Garbage Collector
**Files:**
- `src/spur64.stack/validImage.c` (~492k lines)
- Spur GC implementation

**Strategy:**
- Replace object header manipulation first
- Then allocation routines
- Finally GC marking/sweeping logic
- Critical: extensive memory corruption testing

**Zig Advantages:**
- `@intToPtr`/`@ptrToInt` with explicit unsafe marking
- Clearer pointer arithmetic
- Better alignment control with `@alignOf`

#### 7.3 Stack Management
**Files:**
- Stack frame manipulation in interpreter
- Context switching code

**Replacement:** `zig/vm/stack.zig`
- Type-safe stack frame representation
- Bounds-checked stack operations
- Better debugging of stack corruption

#### 7.4 Primitive Table
**Current:** Large switch/table of VM primitives
**Replacement:** Comptime-generated primitive dispatch

```zig
const primitives = [_]PrimitiveFn{
    primitiveAdd,
    primitiveSubtract,
    // ... hundreds of primitives
};

fn callPrimitive(index: usize) void {
    if (index >= primitives.len) return primitiveFail();
    primitives[index]();
}
```

#### 7.5 JIT Compiler (Advanced)
**Files:**
- `src/spur64.cog/cogit.c` (includes processor-specific files)
- `src/spur64.cog/cogitIA32.c`, `cogitARMv8.c`, etc.

**Strategy:**
- This is the most complex component
- Consider reimplementing code generation in Zig
- Better type safety for machine code emission
- Potentially use LLVM bindings instead of hand-written JIT

**Very Long-Term:** Could replace entire JIT with LLVM backend

### Success Criteria
- [ ] VM passes entire Smalltalk test suite
- [ ] Performance benchmarks show improvement or parity
- [ ] Memory safety improved (fewer segfaults)
- [ ] GC bugs reduced
- [ ] Easier to maintain and debug

### Testing Strategy
1. **Differential testing:** Run same Smalltalk code on C and Zig VMs, compare results
2. **Fuzzing:** Random bytecode sequences to find edge cases
3. **GC stress testing:** Force GC cycles with various heap configurations
4. **Performance regression tests:** Automated benchmarks

### Critical: Maintaining C Reference Implementation

**IMPORTANT:** During Phase 7, maintain the C implementation as a reference:

```c
// In build, support both implementations
#ifdef USE_ZIG_INTERPRETER
  #include "zig/vm/interpreter.h"
#else
  #include "src/spur64.stack/interp.h"
#endif
```

**Why:**
- Can run A/B tests on same Smalltalk code
- Easy rollback if critical bugs found
- Reference for behavioral questions
- Confidence during gradual transition

**When to remove C code:**
- Only after 6+ months of production use with Zig implementation
- All known edge cases handled
- Performance benchmarks validated
- Community comfortable with change

---

## Technical Patterns

### C/Zig Interop Pattern

```zig
// zig_component.zig
const c = @cImport({
    @cInclude("sq.h");
});

// Export C-compatible function
export fn zigFunction(arg: c.sqInt) c.sqInt {
    // Implementation in Zig
    return arg + 1;
}
```

```c
// In C code
#include "zig_component.h"
extern sqInt zigFunction(sqInt arg);

void cFunction() {
    sqInt result = zigFunction(42);
}
```

### Plugin Template

```zig
// Standard plugin structure
const std = @import("std");
const c = @cImport({
    @cInclude("sqVirtualMachine.h");
    @cInclude("sqPlatformSpecific.h");
});

var interpreterProxy: *c.VirtualMachine = undefined;

export fn setInterpreter(anInterpreter: *c.VirtualMachine) c.sqInt {
    interpreterProxy = anInterpreter;
    return 1;
}

export fn primitiveFunctionName() void {
    // Access Smalltalk stack via interpreterProxy
    const arg = interpreterProxy.stackValue(0);

    // Do work
    const result = // ...

    // Return result
    interpreterProxy.popthenPush(1, result);
}
```

### Build Integration

```zig
// build.zig
const std = @import("std");

pub fn build(b: *std.Build) void {
    // Define targets
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    // Compile existing C code (gradually being replaced with Zig)
    const vm_lib = b.addStaticLibrary(.{
        .name = "vm_core",
        .target = target,
        .optimize = optimize,
    });
    vm_lib.addCSourceFiles(&.{
        "src/spur64.stack/interp.c",
        // ... other C files (will be replaced incrementally)
    }, &.{"-std=c99"});
    vm_lib.linkLibC();

    // Compile Zig components
    const zig_platform = b.addStaticLibrary(.{
        .name = "platform_zig",
        .root_source_file = .{ .path = "zig/platform/main.zig" },
        .target = target,
        .optimize = optimize,
    });

    // Link together
    const vm = b.addExecutable(.{
        .name = "squeak",
        .target = target,
        .optimize = optimize,
    });
    vm.linkLibrary(vm_lib);
    vm.linkLibrary(zig_platform);
}
```

---

## Testing Strategy

### Unit Tests (Zig)
```zig
test "UUID generation produces valid format" {
    const uuid = generateUUID();
    try std.testing.expect(uuid.len == 36);
    try std.testing.expect(uuid[8] == '-');
}
```

### Integration Tests
1. Run existing Smalltalk test suite against Zig-enabled VM
2. Compare outputs with C-only build (bit-for-bit identical)
3. Performance benchmarks (no regression > 5%)

### Regression Prevention
- Keep C implementations alongside Zig during transition
- Build flag to toggle: `USE_ZIG_COMPONENTS=1`
- Automated A/B testing in CI

---

## Risk Mitigation

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| Zig language changes (pre-1.0) | Medium | Medium | Pin to specific Zig version, upgrade deliberately |
| Performance regression | Low | High | Extensive benchmarking, keep C fallback |
| Platform compatibility issues | Medium | Medium | Test on all platforms early, leverage Zig cross-compilation |
| Team learning curve | Medium | Low | Start with small components, document patterns |
| Build complexity | Low | Medium | Maintain parallel builds during transition |
| Header incompatibilities | Low | High | Test `@cImport()` in Phase 1, fix early |
| **Interpreter behavioral differences** | **Medium-High** | **Critical** | Differential testing, maintain C reference, incremental replacement |
| **GC bugs introduced** | **Medium** | **Critical** | Extensive stress testing, memory debugging tools, gradual rollout |
| **JIT code generation errors** | **High** | **Critical** | Comprehensive test suite, comparison with C JIT output, consider LLVM |

---

## Success Metrics

### Phase-by-Phase
- **Phase 1:** Zig builds and imports headers (0% code replacement)
- **Phase 2:** 3 utility files replaced (~1,000 LOC)
- **Phase 3:** 1 plugin in Zig (~2% of plugins)
- **Phase 4:** Platform layer mostly Zig (~10% of total codebase)
- **Phase 5:** Unified build system
- **Phase 6:** New features default to Zig (~20% of total codebase)
- **Phase 7:** Core VM in Zig - interpreter, GC, JIT (50-80% of total codebase)

### Overall Success Criteria

**Phases 1-6:**
- [ ] VM passes all Smalltalk tests with Zig components
- [ ] No performance regression (within 5%)
- [ ] Reduced platform-specific #ifdefs
- [ ] Faster build times
- [ ] Easier cross-compilation
- [ ] New contributors find Zig code clearer
- [ ] Reduced memory safety issues in platform code

**Phase 7 (Core VM):**
- [ ] Zig interpreter produces identical results to C interpreter
- [ ] GC behavior matches C implementation (no new memory leaks or corruption)
- [ ] Performance improvement or parity with C VM
- [ ] Can run production Smalltalk images (Squeak, Cuis) without issues
- [ ] JIT (if replaced) generates correct machine code for all platforms
- [ ] Significantly reduced crash rate from memory issues
- [ ] Easier to add new bytecodes and primitives

---

## Rollback Plan

Each phase maintains the C implementation:

```c
#ifdef USE_ZIG_PLATFORM
  #include "zig/platform.h"
#else
  // Original C implementation
#endif
```

If critical issues arise:
1. Set `USE_ZIG_PLATFORM=0` in build
2. Revert to pure C build
3. No data loss or compatibility issues

---

## Timeline Summary

| Phase | Duration | Risk Level | Key Deliverable |
|-------|----------|------------|-----------------|
| 1: Foundation | 2 weeks | Low | Zig builds, headers import |
| 2: Utilities | 3 weeks | Low | 3 utilities in Zig |
| 3: Plugin | 4 weeks | Medium | Full plugin replacement |
| 4: Platform Layer | 5 weeks | Medium | Core platform code in Zig |
| 5: Build System | 4 weeks | Medium | Unified build.zig |
| 6: Advanced | 6 weeks | Medium-High | FFI, profiling tools |
| 7: Core VM | 20+ weeks | **HIGH** | Interpreter, GC, JIT in Zig |

**Milestones:**
- **Production-ready Zig integration (Phases 1-5):** 18 weeks (~4.5 months)
- **Significant Zig adoption (through Phase 6):** 24 weeks (~6 months)
- **Core VM in Zig (Phase 7 complete):** 44+ weeks (~11 months)

---

## Getting Started

### Immediate Next Steps

1. **Install Zig:**
   ```bash
   cd /Users/Shared/Projects/opensmalltalk-vm
   curl -O https://ziglang.org/download/0.13.0/zig-macos-aarch64-0.13.0.tar.xz
   tar xf zig-macos-aarch64-0.13.0.tar.xz
   export PATH=$PATH:$PWD/zig-macos-aarch64-0.13.0
   ```

2. **Create basic build.zig:**
   ```bash
   zig init
   ```

3. **Test header imports:**
   Create `zig/test_headers.zig` and try importing `sq.h`

4. **Pick first target:**
   Recommend starting with `sqTextEncoding.c` (Phase 2.1)

---

## Resources

- **Zig Documentation:** https://ziglang.org/documentation/master/
- **C Interop Guide:** https://ziglang.org/documentation/master/#C
- **Build System:** https://ziglang.org/learn/build-system/
- **Cross-compilation:** https://ziglang.org/learn/cross-compilation/

---

## Questions to Resolve

Before starting Phase 2:
- [ ] Do we want to maintain C fallbacks indefinitely or eventually remove?
- [ ] Which plugin should we target first (UUID vs MD5 vs FileAttributes)?
- [ ] Should we target other platforms (Linux/Windows) in parallel or sequentially?
- [ ] How aggressive should we be with Phase 7 (core VM replacement)?
- [ ] Should we consider LLVM backend instead of hand-written JIT in Phase 7.5?

Before starting Phase 7:
- [ ] Have we gained sufficient confidence from Phases 1-6?
- [ ] Do we have comprehensive test coverage of interpreter behavior?
- [ ] Should we implement a reference interpreter in Zig first, before replacing production code?

---

**Last Updated:** 2025-10-20
**Status:** Planning Phase (Updated to reflect all C code is now hand-maintained)
**Next Review:** After Phase 1 completion
