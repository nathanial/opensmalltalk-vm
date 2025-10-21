/*
 * BitBltTemplates.hpp - C++ Template-based BitBlt Implementation
 *
 * This file provides a modern C++ template-based approach to BitBlt operations,
 * replacing the macro-based code generation with type-safe templates and
 * compile-time specialization.
 *
 * Copyright © 2025 OpenSmalltalk VM
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.
 */

#ifndef BITBLTTEMPLATES_HPP_
#define BITBLTTEMPLATES_HPP_

#include "BitBltDispatch.h"
#include <stdint.h>
#include <cstring>
#include <type_traits>

namespace BitBlt {

// ============================================================================
// Pixel Format Traits
// ============================================================================

/**
 * Compile-time traits for different pixel formats.
 * Specialized for each supported bit depth.
 */
template<unsigned BitsPerPixel>
struct PixelFormatTraits {
    static_assert(BitsPerPixel == 0, "Unsupported pixel format");
};

// Specialization for 1 bpp
template<>
struct PixelFormatTraits<1> {
    using StorageType = uint32_t;  // Packed into words
    static constexpr unsigned BPP = 1;
    static constexpr unsigned PIXELS_PER_WORD = 32;
    static constexpr uint32_t PIXEL_MASK = 0x1;
};

// Specialization for 2 bpp
template<>
struct PixelFormatTraits<2> {
    using StorageType = uint32_t;  // Packed into words
    static constexpr unsigned BPP = 2;
    static constexpr unsigned PIXELS_PER_WORD = 16;
    static constexpr uint32_t PIXEL_MASK = 0x3;
};

// Specialization for 4 bpp
template<>
struct PixelFormatTraits<4> {
    using StorageType = uint32_t;  // Packed into words
    static constexpr unsigned BPP = 4;
    static constexpr unsigned PIXELS_PER_WORD = 8;
    static constexpr uint32_t PIXEL_MASK = 0xF;
};

// Specialization for 8 bpp
template<>
struct PixelFormatTraits<8> {
    using StorageType = uint8_t;
    static constexpr unsigned BPP = 8;
    static constexpr unsigned PIXELS_PER_WORD = 4;
    static constexpr uint32_t PIXEL_MASK = 0xFF;
};

// Specialization for 16 bpp
template<>
struct PixelFormatTraits<16> {
    using StorageType = uint16_t;
    static constexpr unsigned BPP = 16;
    static constexpr unsigned PIXELS_PER_WORD = 2;
    static constexpr uint32_t PIXEL_MASK = 0xFFFF;
};

// Specialization for 32 bpp
template<>
struct PixelFormatTraits<32> {
    using StorageType = uint32_t;
    static constexpr unsigned BPP = 32;
    static constexpr unsigned PIXELS_PER_WORD = 1;
    static constexpr uint32_t PIXEL_MASK = 0xFFFFFFFF;
};

// ============================================================================
// Pixel Accessor - Handles bit-packed and byte-aligned pixels
// ============================================================================

/**
 * Generic pixel accessor template.
 * Provides unified interface for reading/writing pixels of different depths.
 */
template<unsigned BPP, typename Enable = void>
class PixelAccessor;

/**
 * Specialization for byte-aligned pixels (8, 16, 32 bpp)
 */
template<unsigned BPP>
class PixelAccessor<BPP, typename std::enable_if<(BPP >= 8)>::type> {
public:
    using Traits = PixelFormatTraits<BPP>;
    using Storage = typename Traits::StorageType;

    static inline uint32_t read(const Storage* buffer, uint32_t x) {
        return buffer[x];
    }

    static inline void write(Storage* buffer, uint32_t x, uint32_t pixel) {
        buffer[x] = static_cast<Storage>(pixel);
    }
};

/**
 * Specialization for bit-packed pixels (1, 2, 4 bpp)
 */
template<unsigned BPP>
class PixelAccessor<BPP, typename std::enable_if<(BPP < 8)>::type> {
public:
    using Traits = PixelFormatTraits<BPP>;
    using Storage = uint32_t;

    static inline uint32_t read(const Storage* buffer, uint32_t x, bool msb = true) {
        const uint32_t wordIndex = (x * BPP) / 32;
        const uint32_t bitOffset = (x * BPP) % 32;
        const uint32_t word = buffer[wordIndex];

        if (msb) {
            // Big-endian: highest bit first
            const uint32_t shift = 32 - BPP - bitOffset;
            return (word >> shift) & Traits::PIXEL_MASK;
        } else {
            // Little-endian: lowest bit first
            return (word >> bitOffset) & Traits::PIXEL_MASK;
        }
    }

    static inline void write(Storage* buffer, uint32_t x, uint32_t pixel, bool msb = true) {
        const uint32_t wordIndex = (x * BPP) / 32;
        const uint32_t bitOffset = (x * BPP) % 32;
        uint32_t word = buffer[wordIndex];

        if (msb) {
            // Big-endian: highest bit first
            const uint32_t shift = 32 - BPP - bitOffset;
            const uint32_t mask = Traits::PIXEL_MASK << shift;
            word = (word & ~mask) | ((pixel << shift) & mask);
        } else {
            // Little-endian: lowest bit first
            const uint32_t mask = Traits::PIXEL_MASK << bitOffset;
            word = (word & ~mask) | ((pixel << bitOffset) & mask);
        }

        buffer[wordIndex] = word;
    }
};

// ============================================================================
// Combination Rules - Template-based implementations
// ============================================================================

/**
 * Base template for combination rules.
 * Specialized for each rule type.
 */
template<combination_rule_t Rule>
struct CombinationRule {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        // Default: just return dest unchanged
        return dest;
    }
};

// Specializations for each combination rule
template<>
struct CombinationRule<CR_clearWord> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        (void)src; (void)dest;
        return 0;
    }
};

template<>
struct CombinationRule<CR_bitAnd> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return src & dest;
    }
};

template<>
struct CombinationRule<CR_bitAndInvert> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return src & ~dest;
    }
};

template<>
struct CombinationRule<CR_sourceWord> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        (void)dest;
        return src;
    }
};

template<>
struct CombinationRule<CR_bitInvertAnd> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return ~src & dest;
    }
};

template<>
struct CombinationRule<CR_destinationWord> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        (void)src;
        return dest;
    }
};

template<>
struct CombinationRule<CR_bitXor> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return src ^ dest;
    }
};

template<>
struct CombinationRule<CR_bitOr> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return src | dest;
    }
};

template<>
struct CombinationRule<CR_bitInvertAndInvert> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return ~src & ~dest;
    }
};

template<>
struct CombinationRule<CR_bitInvertXor> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return ~src ^ dest;
    }
};

template<>
struct CombinationRule<CR_bitInvertDestination> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        (void)src;
        return ~dest;
    }
};

template<>
struct CombinationRule<CR_bitOrInvert> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return src | ~dest;
    }
};

template<>
struct CombinationRule<CR_bitInvertSource> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        (void)dest;
        return ~src;
    }
};

template<>
struct CombinationRule<CR_bitInvertOr> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return ~src | dest;
    }
};

template<>
struct CombinationRule<CR_bitInvertOrInvert> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return ~src | ~dest;
    }
};

template<>
struct CombinationRule<CR_addWord> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return src + dest;
    }
};

template<>
struct CombinationRule<CR_subWord> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return src - dest;
    }
};

// ============================================================================
// BitBlt Operation Templates
// ============================================================================

/**
 * Generic BitBlt operation template.
 * @tparam SrcBPP Source bits per pixel
 * @tparam DestBPP Destination bits per pixel
 * @tparam Rule Combination rule to apply
 */
template<unsigned SrcBPP, unsigned DestBPP, combination_rule_t Rule>
class BitBltOperation {
public:
    using SrcTraits = PixelFormatTraits<SrcBPP>;
    using DestTraits = PixelFormatTraits<DestBPP>;
    using SrcAccessor = PixelAccessor<SrcBPP>;
    using DestAccessor = PixelAccessor<DestBPP>;

    /**
     * Execute the BitBlt operation.
     */
    static void execute(const operation_t* op) {
        auto srcBits = static_cast<const typename SrcTraits::StorageType*>(op->src.bits);
        auto destBits = static_cast<typename DestTraits::StorageType*>(op->dest.bits);

        const uint32_t srcPitch = op->src.pitch / sizeof(typename SrcTraits::StorageType);
        const uint32_t destPitch = op->dest.pitch / sizeof(typename DestTraits::StorageType);

        const bool srcMSB = op->src.msb;
        const bool destMSB = op->dest.msb;

        const uint32_t srcX = op->src.x;
        const uint32_t srcY = op->src.y;
        const uint32_t destX = op->dest.x;
        const uint32_t destY = op->dest.y;

        const uint32_t width = op->width;
        const uint32_t height = op->height;

        // Iterate over rows
        for (uint32_t y = 0; y < height; y++) {
            const uint32_t srcRow = srcY + y;
            const uint32_t destRow = destY + y;

            // Iterate over pixels in row
            for (uint32_t x = 0; x < width; x++) {
                // Read source pixel
                uint32_t srcPixel;
                if constexpr (SrcBPP < 8) {
                    srcPixel = SrcAccessor::read(srcBits + srcRow * srcPitch, srcX + x, srcMSB);
                } else {
                    srcPixel = SrcAccessor::read(srcBits + srcRow * srcPitch, srcX + x);
                }

                // Read destination pixel
                uint32_t destPixel;
                if constexpr (DestBPP < 8) {
                    destPixel = DestAccessor::read(destBits + destRow * destPitch, destX + x, destMSB);
                } else {
                    destPixel = DestAccessor::read(destBits + destRow * destPitch, destX + x);
                }

                // Apply combination rule
                uint32_t result = CombinationRule<Rule>::apply(srcPixel, destPixel);

                // Write result
                if constexpr (DestBPP < 8) {
                    DestAccessor::write(destBits + destRow * destPitch, destX + x, result, destMSB);
                } else {
                    DestAccessor::write(destBits + destRow * destPitch, destX + x, result);
                }
            }
        }
    }
};

// ============================================================================
// Optimized Specializations
// ============================================================================

/**
 * Optimized specialization for 32bpp -> 32bpp with simple copy.
 * Uses memcpy for maximum performance when possible.
 */
template<>
class BitBltOperation<32, 32, CR_sourceWord> {
public:
    static void execute(const operation_t* op) {
        auto srcBits = static_cast<const uint32_t*>(op->src.bits);
        auto destBits = static_cast<uint32_t*>(op->dest.bits);

        const uint32_t srcPitch = op->src.pitch / sizeof(uint32_t);
        const uint32_t destPitch = op->dest.pitch / sizeof(uint32_t);

        const uint32_t srcX = op->src.x;
        const uint32_t srcY = op->src.y;
        const uint32_t destX = op->dest.x;
        const uint32_t destY = op->dest.y;

        const uint32_t width = op->width;
        const uint32_t height = op->height;

        for (uint32_t y = 0; y < height; y++) {
            const uint32_t* srcRow = srcBits + (srcY + y) * srcPitch + srcX;
            uint32_t* destRow = destBits + (destY + y) * destPitch + destX;
            std::memcpy(destRow, srcRow, width * sizeof(uint32_t));
        }
    }
};

/**
 * Optimized specialization for 32bpp clear (fill with 0).
 */
template<>
class BitBltOperation<32, 32, CR_clearWord> {
public:
    static void execute(const operation_t* op) {
        auto destBits = static_cast<uint32_t*>(op->dest.bits);
        const uint32_t destPitch = op->dest.pitch / sizeof(uint32_t);
        const uint32_t destX = op->dest.x;
        const uint32_t destY = op->dest.y;
        const uint32_t width = op->width;
        const uint32_t height = op->height;

        for (uint32_t y = 0; y < height; y++) {
            uint32_t* destRow = destBits + (destY + y) * destPitch + destX;
            std::memset(destRow, 0, width * sizeof(uint32_t));
        }
    }
};

// ============================================================================
// C API Bridge
// ============================================================================

/**
 * C-compatible wrapper functions for template instantiations.
 * These provide the bridge between the C API and C++ templates.
 */
extern "C" {

// 32bpp -> 32bpp operations
void bitblt_32_32_clearWord(operation_t* op, uint32_t flags);
void bitblt_32_32_sourceWord(operation_t* op, uint32_t flags);
void bitblt_32_32_bitAnd(operation_t* op, uint32_t flags);
void bitblt_32_32_bitOr(operation_t* op, uint32_t flags);
void bitblt_32_32_bitXor(operation_t* op, uint32_t flags);

// 8bpp -> 32bpp operations
void bitblt_8_32_sourceWord(operation_t* op, uint32_t flags);

// 16bpp -> 32bpp operations
void bitblt_16_32_sourceWord(operation_t* op, uint32_t flags);

// More combinations can be added as needed...

} // extern "C"

} // namespace BitBlt

#endif // BITBLTTEMPLATES_HPP_
