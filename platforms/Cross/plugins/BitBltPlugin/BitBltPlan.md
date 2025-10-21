# BitBlt Template Conversion Master Plan

## Executive Summary

This document outlines a comprehensive plan to convert the remaining BitBlt plugin implementations from manual C code to modern C++ templates, building on the successful conversion of basic operations, alpha blending, and RGB operations.

**Current Status**: ~35% converted to templates
**Target**: 80-90% template-based (keeping SIMD and platform-specific optimizations in C)
**Estimated Impact**: -2000 lines of C code, +800 lines of templates (net -1200 lines)

---

## Table of Contents

1. [Current State Analysis](#current-state-analysis)
2. [Conversion Opportunities](#conversion-opportunities)
3. [Phased Implementation Plan](#phased-implementation-plan)
4. [Technical Architecture](#technical-architecture)
5. [SIMD Optimization Strategy](#simd-optimization-strategy)
6. [Risk Assessment](#risk-assessment)
7. [Success Metrics](#success-metrics)

---

## Current State Analysis

### ✅ Already Converted (35%)

**Basic Bit Operations** (17 operations):
- CR_clearWord, CR_sourceWord, CR_destinationWord
- CR_bitAnd, CR_bitOr, CR_bitXor
- CR_bitAndInvert, CR_bitInvertAnd
- CR_bitInvertSource, CR_bitInvertDestination
- CR_bitInvertXor, CR_bitOrInvert, CR_bitInvertOr
- CR_bitInvertAndInvert, CR_bitInvertOrInvert
- CR_addWord, CR_subWord

**Alpha Operations** (2 operations):
- CR_alphaBlend
- CR_alphaBlendConst

**RGB Operations** (6 operations):
- CR_rgbAdd, CR_rgbSub, CR_rgbMul
- CR_rgbMax, CR_rgbMin, CR_rgbMinInvert

**Total**: 25 operations templated across multiple pixel formats

### 🔄 Remaining Manual C Code (65%)

#### Combination Rules Not Yet Templated (20 operations)

**Pixel Operations**:
- CR_pixPaint (25) - Paint with transparency
- CR_pixMask (26) - Masking operation
- CR_pixSwap (38) - Swap pixels
- CR_pixClear (39) - Clear with pixel value

**Advanced Alpha Operations** (8):
- CR_alphaPaintConst (31)
- CR_alphaBlendScaled (34)
- CR_alphaBlendScaled_alt1 (35)
- CR_alphaBlendScaled_alt2 (36)
- CR_alphaScale (42)
- CR_alphaUnscale (43)
- CR_alphaBlendUnscaled (44)
- CR_fixAlpha (40)

**Component Alpha**:
- CR_rgbComponentAlpha (41) - Component-wise alpha blending

**Utility Operations**:
- CR_rgbDiff (32) - RGB difference
- CR_tallyIntoMap (33) - Histogram tallying
- CR_OLDtallyIntoMap (23) - Legacy version
- CR_OLDrgbDiff (22) - Legacy version

**Alternate Destination** (compatibility):
- CR_destinationWord_alt1 (15)
- CR_destinationWord_alt2 (16)
- CR_destinationWord_alt3 (17)

#### Special Fast Paths (Manual C)

**Pixel Format Specific** (~310 lines):
- `fastPathClearWord4` - 4bpp clear operation
- `fastPathClearWord8` - 8bpp clear operation
- `fastPathClearWord32` - 32bpp clear operation (could use template)
- `fastPathSourceWord0_32_scalar` - Scalar halftone fill
- `fastPathSourceWord8_32` - 8→32 conversion with color map
- `fastPathSourceWord32_32` - 32→32 copy with overlap handling

**Helper Operations** (~200 lines):
- `fastPathRightToLeft` - Overlap handling (horizontal)
- `fastPathBottomToTop` - Overlap handling (vertical)
- `fastPathDepthConv` - Generic depth conversion fallback
- `fastPathEarlyHalftone` - Halftone preprocessing
- `fastPathNoOp` - No-operation (compatibility)

#### Platform-Specific SIMD (~3000 lines)
- ARM NEON implementations (BitBltArmSimd.c)
- ARM64 optimizations (BitBltArm64.c)
- **Note**: Keep these! They're hand-optimized SIMD

---

## Conversion Opportunities

### Phase 1: High-Value, Low-Risk (Immediate) 🎯

**Target**: Pixel operations and simple alpha variants
**Effort**: 2-3 days
**Impact**: -150 lines, +60 template lines

#### 1.1 Pixel Operations (4 operations)
```cpp
template<> struct CombinationRule<CR_pixPaint> { ... };
template<> struct CombinationRule<CR_pixMask> { ... };
template<> struct CombinationRule<CR_pixSwap> { ... };
template<> struct CombinationRule<CR_pixClear> { ... };
```

**Benefit**:
- Clean template implementation
- Type-safe pixel manipulation
- Works across all pixel depths

**Complexity**: LOW
- Similar to existing bit operations
- No special handling needed
- Straightforward bit manipulation

#### 1.2 RGB Utilities (2 operations)
```cpp
template<> struct CombinationRule<CR_rgbDiff> { ... };
template<> struct CombinationRule<CR_rgbComponentAlpha> { ... };
```

**Benefit**:
- Complete RGB operation coverage
- Reuse existing RGBHelper
- Type-safe component operations

**Complexity**: LOW
- Extend existing RGBHelper
- Simple component arithmetic
- Already have patterns established

#### 1.3 Bit-Packed Clear Operations
Convert `fastPathClearWord4` and `fastPathClearWord8` to templates:

```cpp
template<unsigned BPP>
class BitBltClearOperation {
    // Specialized for 1, 2, 4, 8 bpp
    // Handles bit-packing automatically
};
```

**Benefit**:
- Eliminate duplicate code
- Single template for all bit depths
- Better compiler optimization

**Complexity**: MEDIUM
- Bit manipulation patterns
- Edge case handling
- But we already have PixelAccessor framework

---

### Phase 2: Advanced Alpha Operations (Medium Priority) 🎨

**Target**: Scaled alpha, alpha painting, component alpha
**Effort**: 3-4 days
**Impact**: -200 lines, +80 template lines

#### 2.1 Alpha Scaling Operations (4 operations)
```cpp
template<> struct CombinationRule<CR_alphaScale> {
    static inline uint32_t apply(uint32_t src, uint32_t dest, uint8_t scaleFactor) {
        // Scale alpha channel by factor
    }
};

template<> struct CombinationRule<CR_alphaUnscale> { ... };
template<> struct CombinationRule<CR_alphaBlendScaled> { ... };
template<> struct CombinationRule<CR_alphaBlendUnscaled> { ... };
```

**Benefit**:
- Complete alpha manipulation suite
- Reuse AlphaBlendHelper
- Consistent scaling across formats

**Complexity**: MEDIUM
- Need to handle scale parameters
- May require operation_t context
- Similar to alphaBlendConst pattern

#### 2.2 Alpha Painting (2 operations)
```cpp
template<> struct CombinationRule<CR_alphaPaintConst> { ... };
template<> struct CombinationRule<CR_fixAlpha> { ... };
```

**Benefit**:
- Complete painting operations
- Support UI rendering workflows
- Type-safe alpha manipulation

**Complexity**: LOW-MEDIUM
- Similar to alphaBlend
- May need destination alpha preservation
- Well-defined algorithms

#### 2.3 Component Alpha Blending
```cpp
template<> struct CombinationRule<CR_rgbComponentAlpha> {
    static inline uint32_t apply(uint32_t src, uint32_t dest, operation_t* op) {
        // Use op->componentAlpha gamma tables
        // Blend each component separately with its own alpha
    }
};
```

**Benefit**:
- Professional compositing support
- Gamma-correct blending
- Font rendering optimization

**Complexity**: HIGH
- Requires gamma table access
- Per-component alpha channels
- More complex than standard alpha blend

---

### Phase 3: Pixel Format Conversions (High Impact) 🔄

**Target**: Depth conversion fast paths
**Effort**: 4-5 days
**Impact**: -300 lines, +150 template lines

#### 3.1 Template-Based Depth Conversion

Replace manual format conversions with:

```cpp
template<unsigned SrcBPP, unsigned DestBPP>
class PixelFormatConverter {
    static inline uint32_t convert(uint32_t srcPixel) {
        if constexpr (SrcBPP == DestBPP) {
            return srcPixel;  // No conversion
        } else if constexpr (SrcBPP < DestBPP) {
            return expandPixel<SrcBPP, DestBPP>(srcPixel);
        } else {
            return compressPixel<SrcBPP, DestBPP>(srcPixel);
        }
    }
};
```

**Common Conversions**:
- 8bpp → 32bpp (index to RGB via color map)
- 16bpp → 32bpp (RGB565 to ARGB8888)
- 32bpp → 16bpp (ARGB8888 to RGB565)
- 32bpp → 8bpp (RGB to index via dithering)
- 1/2/4bpp ↔ 32bpp (bit-packed to ARGB)

**Benefit**:
- Single template handles all conversions
- Compile-time optimization
- Automatic SIMD opportunities
- Eliminates ~300 lines of duplicate code

**Complexity**: HIGH
- Color map integration
- Dithering for downsampling
- Bit-packing/unpacking
- Endianness handling

#### 3.2 Color Map Templates

```cpp
template<ColorMapType MapType>
class ColorMapApplicator {
    // Specialized for:
    // - NO_COLOR_MAP (identity)
    // - INDEXED_1BIT (2-color map)
    // - INDEXED_8BIT (256-color map)
    // - DIRECT_COLOR (RGB shift/mask)
};
```

**Benefit**:
- Type-safe color mapping
- Compile-time dispatch
- Easier to optimize
- Reusable across operations

**Complexity**: MEDIUM
- Multiple map types
- Lookup table access
- Bit manipulation

---

### Phase 4: Halftone Operations (Specialized) 🎭

**Target**: Halftone preprocessing and application
**Effort**: 2-3 days
**Impact**: -100 lines, +50 template lines

#### 4.1 Halftone Templates

```cpp
template<HalftoneType Type>
class HalftoneApplicator {
    // SCALAR: Single color value
    // VECTOR: Pattern array
    // NONE: Pass-through
};

template<combination_rule_t Rule, HalftoneType HT>
class BitBltOperationWithHalftone {
    // Combines operation with halftone
};
```

**Benefit**:
- Clean separation of concerns
- Compile-time halftone dispatch
- Works with all combination rules

**Complexity**: MEDIUM
- Pattern handling
- Coordinate-based selection
- Integration with operations

---

### Phase 5: SIMD Template Specializations (Performance) ⚡

**Target**: Vectorized operations using templates
**Effort**: 5-7 days
**Impact**: Significant performance gains for bulk operations

#### 5.1 SSE2/AVX2 Specializations (x86/x64)

```cpp
#ifdef __SSE2__
template<>
class BitBltOperation<32, 32, CR_sourceWord> {
    static void execute(const operation_t* op) {
        // Process 4 pixels at once with SSE2
        __m128i* src = (__m128i*)op->src.bits;
        __m128i* dest = (__m128i*)op->dest.bits;

        for (size_t i = 0; i < pixelCount / 4; i++) {
            _mm_store_si128(&dest[i], _mm_load_si128(&src[i]));
        }
        // Handle remaining pixels with scalar code
    }
};
#endif

#ifdef __AVX2__
template<>
class BitBltOperation<32, 32, CR_alphaBlend> {
    static void execute(const operation_t* op) {
        // Process 8 pixels at once with AVX2
        __m256i* src = (__m256i*)op->src.bits;
        __m256i* dest = (__m256i*)op->dest.bits;
        // ... vectorized alpha blending ...
    }
};
#endif
```

**Benefit**:
- 4-8x speedup for bulk operations
- Automatic fallback to scalar
- Type-safe SIMD code
- Easier to maintain than intrinsics in C

**Complexity**: HIGH
- SIMD intrinsics expertise required
- Alignment handling
- Edge case management
- Testing across platforms

#### 5.2 NEON Specializations (ARM/ARM64)

```cpp
#ifdef __ARM_NEON
template<>
class BitBltOperation<32, 32, CR_alphaBlend> {
    static void execute(const operation_t* op) {
        // Process 4 pixels at once with NEON
        uint32x4_t* src = (uint32x4_t*)op->src.bits;
        uint32x4_t* dest = (uint32x4_t*)op->dest.bits;
        // ... NEON alpha blending ...
    }
};
#endif
```

**Benefit**:
- Mobile/embedded performance
- Consistent with existing ARM optimizations
- Template-based selection

**Complexity**: HIGH
- ARM intrinsics
- Platform variations (v7 vs v8)
- Alignment requirements

---

## Phased Implementation Plan

### Timeline Overview

```
Week 1-2:   Phase 1 - Pixel & RGB operations (4-6 operations)
Week 3-4:   Phase 2 - Advanced alpha operations (6-8 operations)
Week 5-7:   Phase 3 - Format conversions (major refactor)
Week 8-9:   Phase 4 - Halftone templates
Week 10-14: Phase 5 - SIMD specializations (optional, high-performance)
```

### Phase 1: Quick Wins (Weeks 1-2) 🎯

**Goal**: Template-ify remaining simple operations

**Tasks**:
1. ✅ Add CR_pixPaint, CR_pixMask, CR_pixSwap, CR_pixClear templates
2. ✅ Add CR_rgbDiff, CR_rgbComponentAlpha templates
3. ✅ Convert fastPathClearWord4/8 to templates
4. ✅ Register new fast paths
5. ✅ Test with Smalltalk rendering

**Deliverables**:
- 6 new CombinationRule specializations
- BitBltClearOperation<BPP> template
- ~150 lines removed, ~60 added
- Documentation updates

**Risk**: LOW - Similar to existing conversions

### Phase 2: Alpha Suite (Weeks 3-4) 🎨

**Goal**: Complete alpha operation coverage

**Tasks**:
1. ✅ Implement alpha scaling operations
2. ✅ Implement alpha painting operations
3. ✅ Implement component alpha blending
4. ✅ Add gamma table support to templates
5. ✅ Test with alpha-heavy Smalltalk code

**Deliverables**:
- 8 new alpha operation templates
- GammaHelper utility class
- ~200 lines removed, ~80 added
- Performance benchmarks

**Risk**: MEDIUM - Component alpha is complex

### Phase 3: Format Conversions (Weeks 5-7) 🔄

**Goal**: Templated pixel format conversion system

**Tasks**:
1. ✅ Design PixelFormatConverter template architecture
2. ✅ Implement color map templates
3. ✅ Convert depth conversion fast paths
4. ✅ Handle dithering for downsampling
5. ✅ Extensive testing across formats

**Deliverables**:
- PixelFormatConverter<SrcBPP, DestBPP> template
- ColorMapApplicator<Type> template
- ~300 lines removed, ~150 added
- Comprehensive test suite

**Risk**: HIGH - Complex interactions, many edge cases

### Phase 4: Halftone (Weeks 8-9) 🎭

**Goal**: Template-based halftone system

**Tasks**:
1. ✅ Design halftone template architecture
2. ✅ Implement HalftoneApplicator<Type>
3. ✅ Integrate with combination rules
4. ✅ Test pattern rendering

**Deliverables**:
- HalftoneApplicator template
- Integration with existing operations
- ~100 lines removed, ~50 added

**Risk**: MEDIUM - Pattern handling complexity

### Phase 5: SIMD Optimizations (Weeks 10-14) ⚡

**Goal**: Vectorized template specializations

**Tasks**:
1. ✅ Implement SSE2 specializations (x86-64)
2. ✅ Implement AVX2 specializations (x86-64)
3. ✅ Implement NEON specializations (ARM)
4. ✅ Benchmark against C SIMD code
5. ✅ Platform testing

**Deliverables**:
- SIMD template specializations
- Performance benchmarks
- Platform-specific optimizations
- 2-8x speedup for bulk operations

**Risk**: HIGH - SIMD expertise required, platform variations

---

## Technical Architecture

### Template Hierarchy

```
BitBltOperation<SrcBPP, DestBPP, Rule>
├── Generic Implementation (all formats)
├── Optimized Specializations
│   ├── BitBltOperation<32, 32, CR_sourceWord>  (memcpy)
│   ├── BitBltOperation<32, 32, CR_clearWord>    (memset)
│   └── BitBltOperation<32, 32, CR_alphaBlend>   (optimized)
└── SIMD Specializations
    ├── SSE2 variants
    ├── AVX2 variants
    └── NEON variants

CombinationRule<Rule>
├── Basic bit operations (17 rules)
├── Alpha operations (10+ rules)
├── RGB operations (7 rules)
└── Pixel operations (4 rules)

Helper Classes
├── AlphaBlendHelper (alpha blending utilities)
├── RGBHelper (RGB component operations)
├── PixelFormatConverter<SrcBPP, DestBPP> (format conversion)
├── ColorMapApplicator<Type> (color mapping)
├── HalftoneApplicator<Type> (halftone patterns)
└── GammaHelper (gamma correction)
```

### Design Patterns

**1. Strategy Pattern**
- CombinationRule templates act as strategies
- BitBltOperation selects strategy at compile time
- Type-safe, zero runtime overhead

**2. Template Specialization**
- Generic template for all cases
- Specialized templates for optimal paths
- Compiler selects best match

**3. SFINAE for SIMD**
```cpp
template<unsigned SrcBPP, unsigned DestBPP, combination_rule_t Rule>
typename std::enable_if<
    SrcBPP == 32 && DestBPP == 32 && has_sse2,
    void
>::type
executeSIMD(const operation_t* op) {
    // SSE2 implementation
}
```

**4. Compile-Time Dispatch**
```cpp
if constexpr (BPP < 8) {
    // Bit-packed code path
} else {
    // Byte-aligned code path
}
```

---

## SIMD Optimization Strategy

### Target Operations for SIMD

**High-Value SIMD Targets**:
1. **CR_sourceWord (copy)** - Memory bandwidth bound
2. **CR_clearWord** - Memory bandwidth bound
3. **CR_alphaBlend** - Compute bound, high parallelism
4. **CR_rgbAdd/Sub/Mul** - Compute bound, vectorizable
5. **CR_bitAnd/Or/Xor** - Trivially vectorizable

### SIMD Performance Model

| Operation | Scalar | SSE2 | AVX2 | NEON | Speedup |
|-----------|--------|------|------|------|---------|
| Copy (32bpp) | 1.0x | 2.5x | 4.0x | 2.5x | High |
| Alpha Blend | 1.0x | 3.5x | 6.0x | 3.0x | Very High |
| RGB Add | 1.0x | 4.0x | 8.0x | 4.0x | Excellent |
| Bit Operations | 1.0x | 4.0x | 8.0x | 4.0x | Excellent |

### SIMD Template Pattern

```cpp
template<unsigned SrcBPP, unsigned DestBPP, combination_rule_t Rule>
class BitBltOperation {
public:
    static void execute(const operation_t* op) {
        #ifdef __AVX2__
        if (canUseAVX2(op)) {
            return executeAVX2(op);
        }
        #endif

        #ifdef __SSE2__
        if (canUseSSE2(op)) {
            return executeSSE2(op);
        }
        #endif

        #ifdef __ARM_NEON
        if (canUseNEON(op)) {
            return executeNEON(op);
        }
        #endif

        // Fallback to scalar
        return executeScalar(op);
    }
};
```

---

## Risk Assessment

### Technical Risks

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| Template code bloat | Medium | Medium | Explicit instantiation, careful specialization |
| Compilation time increase | High | Low | Precompiled headers, distributed builds |
| SIMD alignment issues | Medium | High | Alignment checks, fallback paths |
| Color map complexity | Medium | High | Incremental implementation, extensive testing |
| Halftone edge cases | Low | Medium | Test suite from existing C code |
| Performance regression | Low | High | Benchmarking, profiling, comparison tests |

### Mitigation Strategies

**1. Template Code Bloat**
- Use explicit template instantiation
- Share code through helper classes
- Profile binary size

**2. Compilation Time**
- Use precompiled headers
- Split templates across files
- Parallelize builds

**3. SIMD Issues**
- Runtime CPU detection
- Fallback to scalar code
- Alignment asserts

**4. Compatibility**
- Maintain C API surface
- Keep fast path dispatch unchanged
- Gradual rollout

---

## Success Metrics

### Code Quality Metrics

**Target Improvements**:
- ✅ **Lines of Code**: -1200 lines net (-2000 C, +800 templates)
- ✅ **Code Duplication**: <5% (currently ~30%)
- ✅ **Type Safety**: 100% for templated operations
- ✅ **Test Coverage**: >90% for new templates
- ✅ **Cyclomatic Complexity**: <10 per template function

### Performance Metrics

**Target Benchmarks** (vs current C implementation):
- ✅ **Scalar Operations**: 0-5% faster (better inlining)
- ✅ **SIMD Operations**: 2-8x faster (vectorization)
- ✅ **Compile Time**: <20% increase (acceptable trade-off)
- ✅ **Binary Size**: <10% increase (explicit instantiation)

### Functional Metrics

**Target Coverage**:
- ✅ **Combination Rules**: 38/45 templated (84%)
- ✅ **Pixel Formats**: All (1, 2, 4, 8, 16, 32 bpp)
- ✅ **Color Maps**: All types supported
- ✅ **Halftones**: All types supported
- ✅ **Platform**: Linux, macOS, Windows, ARM

---

## Conclusion

This plan outlines a systematic approach to converting the remaining BitBlt C code to modern C++ templates over approximately 14 weeks. The phased approach allows for:

1. **Incremental Progress** - Each phase delivers value
2. **Risk Management** - Test and validate before proceeding
3. **Flexibility** - Can pause/adjust based on results
4. **Performance Focus** - SIMD phase is optional but high-value

**Recommended Priority**:
1. ✅ Phase 1 (Weeks 1-2) - Quick wins, high value
2. ✅ Phase 2 (Weeks 3-4) - Complete alpha suite
3. ⚠️ Phase 3 (Weeks 5-7) - High impact but complex
4. 🔵 Phase 4 (Weeks 8-9) - Specialized, lower priority
5. ⭐ Phase 5 (Weeks 10-14) - Optional, performance-focused

**Expected Outcome**:
- **80-90% template coverage**
- **Cleaner, more maintainable codebase**
- **Better performance through SIMD**
- **Foundation for future GPU acceleration**

The template-based architecture positions BitBlt for long-term maintainability and opens opportunities for advanced optimizations that would be difficult with manual C code.

---

**Document Version**: 1.0
**Created**: 2025-10-20
**Author**: Claude Code
**Status**: Ready for Implementation
