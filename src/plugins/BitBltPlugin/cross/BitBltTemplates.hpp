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
#include <algorithm>
#include <array>
#include <cstdlib>
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
// Pixel Combination Rules
// ============================================================================

template<>
struct CombinationRule<CR_pixPaint> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return src ? src : dest;
    }
};

template<>
struct CombinationRule<CR_pixMask> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return src ? 0u : dest;
    }
};

template<>
struct CombinationRule<CR_pixClear> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return (src == dest) ? 0u : dest;
    }
};

template<>
struct CombinationRule<CR_pixSwap> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        (void)src;
        return dest;
    }
};

// ============================================================================
// RGB Utility Combination Rules
// ============================================================================

template<>
struct CombinationRule<CR_rgbDiff> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        (void)src;
        return dest;
    }

    template<unsigned DestBPP>
    static inline uint32_t difference(uint32_t src, uint32_t dest) {
        if constexpr (DestBPP < 16) {
            return src == dest ? 0u : 1u;
        } else if constexpr (DestBPP == 16) {
            auto expand5 = [](uint32_t value) -> uint32_t {
                return (value << 3) | (value >> 2);
            };
            uint32_t srcR = expand5((src >> 10) & 0x1F);
            uint32_t srcG = expand5((src >> 5) & 0x1F);
            uint32_t srcB = expand5(src & 0x1F);

            uint32_t destR = expand5((dest >> 10) & 0x1F);
            uint32_t destG = expand5((dest >> 5) & 0x1F);
            uint32_t destB = expand5(dest & 0x1F);

            return static_cast<uint32_t>(
                std::abs(static_cast<int32_t>(srcR) - static_cast<int32_t>(destR)) +
                std::abs(static_cast<int32_t>(srcG) - static_cast<int32_t>(destG)) +
                std::abs(static_cast<int32_t>(srcB) - static_cast<int32_t>(destB))
            );
        } else {
            uint8_t sa, sr, sg, sb;
            uint8_t da, dr, dg, db;
            RGBHelper::extract(src, sa, sr, sg, sb);
            RGBHelper::extract(dest, da, dr, dg, db);
            (void)sa; (void)da;
            return static_cast<uint32_t>(
                std::abs(static_cast<int32_t>(sr) - static_cast<int32_t>(dr)) +
                std::abs(static_cast<int32_t>(sg) - static_cast<int32_t>(dg)) +
                std::abs(static_cast<int32_t>(sb) - static_cast<int32_t>(db))
            );
        }
    }
};

struct ComponentAlphaParams {
    uint32_t modeColor;
    uint8_t  modeAlpha;
    const unsigned char (*gamma)[256];
    const unsigned char (*ungamma)[256];
};

template<>
struct CombinationRule<CR_rgbComponentAlpha> {
    static inline uint32_t apply(uint32_t src, uint32_t dest, const ComponentAlphaParams& params) {
        if (src == 0) {
            return dest;
        }

        auto ungamma = params.ungamma;
        auto gamma = params.gamma;
        auto toLinear = [&](uint8_t value) -> uint32_t {
            return ungamma ? static_cast<uint32_t>((*ungamma)[value]) : static_cast<uint32_t>(value);
        };
        auto toGamma = [&](uint32_t value) -> uint8_t {
            uint32_t clamped = value > 255u ? 255u : value;
            return gamma ? (*gamma)[clamped] : static_cast<uint8_t>(clamped);
        };

        uint8_t srcAlpha = static_cast<uint8_t>(params.modeAlpha & 0xFF);

        uint8_t aB = static_cast<uint8_t>(src & 0xFF);
        uint8_t aG = static_cast<uint8_t>((src >> 8) & 0xFF);
        uint8_t aR = static_cast<uint8_t>((src >> 16) & 0xFF);
        uint8_t aA = static_cast<uint8_t>((src >> 24) & 0xFF);

        if (srcAlpha != 0xFF) {
            auto scale = [srcAlpha](uint8_t value) -> uint8_t {
                return static_cast<uint8_t>((static_cast<uint32_t>(value) * srcAlpha) >> 8);
            };
            aA = scale(aA);
            aR = scale(aR);
            aG = scale(aG);
            aB = scale(aB);
        }

        uint32_t srcColor = params.modeColor;
        uint8_t sB = static_cast<uint8_t>(srcColor & 0xFF);
        uint8_t sG = static_cast<uint8_t>((srcColor >> 8) & 0xFF);
        uint8_t sR = static_cast<uint8_t>((srcColor >> 16) & 0xFF);
        uint8_t sA = static_cast<uint8_t>((srcColor >> 24) & 0xFF);
        (void)sA; // unused in original algorithm

        uint8_t dB = static_cast<uint8_t>(dest & 0xFF);
        uint8_t dG = static_cast<uint8_t>((dest >> 8) & 0xFF);
        uint8_t dR = static_cast<uint8_t>((dest >> 16) & 0xFF);
        uint8_t dA = static_cast<uint8_t>((dest >> 24) & 0xFF);

        auto blendChannel = [&](uint8_t destChan, uint8_t srcChan, uint8_t alphaChan) -> uint8_t {
            if (alphaChan == 0) {
                return destChan;
            }
            uint32_t dLinear = toLinear(destChan);
            uint32_t sLinear = toLinear(srcChan);
            uint32_t blended = ((dLinear * (255u - alphaChan)) + (sLinear * alphaChan)) >> 8;
            return toGamma(blended);
        };

        uint8_t outB = blendChannel(dB, sB, aB);
        uint8_t outG = blendChannel(dG, sG, aG);
        uint8_t outR = blendChannel(dR, sR, aR);

        uint32_t newAlpha = ((static_cast<uint32_t>(dA) * (255u - aA)) >> 8) + aA;
        if (newAlpha > 255u) {
            newAlpha = 255u;
        }

        return (static_cast<uint32_t>(newAlpha) << 24) |
               (static_cast<uint32_t>(outR) << 16) |
               (static_cast<uint32_t>(outG) << 8) |
               static_cast<uint32_t>(outB);
    }
};

// ============================================================================
// Helper Functions for Alpha Blending and RGB Operations
// ============================================================================

/**
 * Alpha blending helper - performs optimized 32-bit ARGB alpha blend.
 * Uses the same algorithm as the existing C code for compatibility.
 *
 * Formula: result = (src * alpha + dest * (255 - alpha) + 255) / 256
 * Optimized to avoid division by processing AG and RB components separately.
 */
struct AlphaBlendHelper {
    /**
     * Blend source and destination pixels with given alpha value.
     * @param src Source pixel (ARGB 32-bit)
     * @param dest Destination pixel (ARGB 32-bit)
     * @param alpha Alpha value (0-255)
     * @return Blended pixel
     */
    static inline uint32_t blend(uint32_t src, uint32_t dest, unsigned int alpha) {
        unsigned int unAlpha = 0xFF - alpha;

        // Extract alpha-green and red-blue components
        // AG = (pixel >> 8) & 0x00FF00FF extracts A and G channels
        // RB = pixel & 0x00FF00FF extracts R and B channels
        uint32_t sAG = ((src >> 8) & 0xFF) | 0xFF0000;  // Keep source green, set alpha to 0xFF
        uint32_t sRB = src & 0xFF00FF;
        uint32_t dAG = (dest >> 8) & 0xFF00FF;
        uint32_t dRB = dest & 0xFF00FF;

        // Blend: (src * alpha + dest * unAlpha + 0xFF00FF)
        // The +0xFF00FF adds rounding for proper division by 256
        uint32_t blendAG = sAG * alpha + dAG * unAlpha + 0xFF00FF;
        uint32_t blendRB = sRB * alpha + dRB * unAlpha + 0xFF00FF;

        // Divide by 256 with rounding: ((x >> 8) + x) >> 8
        blendAG = ((((blendAG >> 8) & 0xFF00FF) + blendAG) >> 8) & 0xFF00FF;
        blendRB = ((((blendRB >> 8) & 0xFF00FF) + blendRB) >> 8) & 0xFF00FF;

        // Recombine: AG goes in upper bytes, RB in lower bytes
        return (blendAG << 8) | blendRB;
    }

    /**
     * Extract alpha channel from 32-bit ARGB pixel.
     */
    static inline unsigned int extractAlpha(uint32_t pixel) {
        return (pixel >> 24) & 0xFF;
    }
};

/**
 * RGB component operations helper.
 */
struct RGBHelper {
    /**
     * Clamp value to 0-255 range.
     */
    static inline uint8_t clamp(int value) {
        if (value < 0) return 0;
        if (value > 255) return 255;
        return static_cast<uint8_t>(value);
    }

    /**
     * Extract ARGB components from 32-bit pixel.
     */
    static inline void extract(uint32_t pixel, uint8_t& a, uint8_t& r, uint8_t& g, uint8_t& b) {
        a = (pixel >> 24) & 0xFF;
        r = (pixel >> 16) & 0xFF;
        g = (pixel >> 8) & 0xFF;
        b = pixel & 0xFF;
    }

    /**
     * Combine ARGB components into 32-bit pixel.
     */
    static inline uint32_t combine(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
        return (static_cast<uint32_t>(a) << 24) |
               (static_cast<uint32_t>(r) << 16) |
               (static_cast<uint32_t>(g) << 8) |
               static_cast<uint32_t>(b);
    }

    /**
     * Add RGB components (saturating).
     */
    static inline uint32_t add(uint32_t src, uint32_t dest) {
        uint8_t sa, sr, sg, sb;
        uint8_t da, dr, dg, db;
        extract(src, sa, sr, sg, sb);
        extract(dest, da, dr, dg, db);
        return combine(
            da,  // Keep dest alpha
            clamp(static_cast<int>(sr) + static_cast<int>(dr)),
            clamp(static_cast<int>(sg) + static_cast<int>(dg)),
            clamp(static_cast<int>(sb) + static_cast<int>(db))
        );
    }

    /**
     * Subtract RGB components (saturating).
     */
    static inline uint32_t sub(uint32_t src, uint32_t dest) {
        uint8_t sa, sr, sg, sb;
        uint8_t da, dr, dg, db;
        extract(src, sa, sr, sg, sb);
        extract(dest, da, dr, dg, db);
        return combine(
            da,  // Keep dest alpha
            clamp(static_cast<int>(sr) - static_cast<int>(dr)),
            clamp(static_cast<int>(sg) - static_cast<int>(dg)),
            clamp(static_cast<int>(sb) - static_cast<int>(db))
        );
    }

    /**
     * Multiply RGB components (normalized to 0-255).
     */
    static inline uint32_t mul(uint32_t src, uint32_t dest) {
        uint8_t sa, sr, sg, sb;
        uint8_t da, dr, dg, db;
        extract(src, sa, sr, sg, sb);
        extract(dest, da, dr, dg, db);
        return combine(
            da,  // Keep dest alpha
            static_cast<uint8_t>((sr * dr) / 255),
            static_cast<uint8_t>((sg * dg) / 255),
            static_cast<uint8_t>((sb * db) / 255)
        );
    }

    /**
     * Maximum of RGB components.
     */
    static inline uint32_t max(uint32_t src, uint32_t dest) {
        uint8_t sa, sr, sg, sb;
        uint8_t da, dr, dg, db;
        extract(src, sa, sr, sg, sb);
        extract(dest, da, dr, dg, db);
        return combine(
            da,  // Keep dest alpha
            sr > dr ? sr : dr,
            sg > dg ? sg : dg,
            sb > db ? sb : db
        );
    }

    /**
     * Minimum of RGB components.
     */
    static inline uint32_t min(uint32_t src, uint32_t dest) {
        uint8_t sa, sr, sg, sb;
        uint8_t da, dr, dg, db;
        extract(src, sa, sr, sg, sb);
        extract(dest, da, dr, dg, db);
        return combine(
            da,  // Keep dest alpha
            sr < dr ? sr : dr,
            sg < dg ? sg : dg,
            sb < db ? sb : db
        );
    }
};

/**
 * Alpha scaling operations helper.
 * Provides pre-multiply and un-multiply operations for alpha blending.
 */
struct AlphaScaleHelper {
    /**
     * Scale (premultiply) RGB components by alpha channel.
     * Formula: RGB' = RGB * alpha / 255
     * Preserves the alpha channel.
     */
    static inline uint32_t scale(uint32_t pixel) {
        unsigned int alpha = (pixel >> 24) & 0xFF;

        // Scale red and blue components
        uint32_t rb = ((pixel & 0xFF00FF) * alpha) + 0x800080;

        // Divide by 255: (x + ((x >> 8) & 0xFF00FF)) >> 8
        rb = (((rb + (((rb - 0x10001) >> 8) & 0xFF00FF))) >> 8) & 0xFF00FF;

        // Scale green component
        uint32_t g = ((pixel & 0xFF00) * alpha) + 0x8000;

        // Divide by 255
        g = (((g + (((g - 0x100) >> 8) & 0xFF00))) >> 8) & 0xFF00;

        // Combine components and preserve alpha
        return (g | rb) | (pixel & 0xFF000000U);
    }

    /**
     * Unscale (divide) RGB components by alpha channel.
     * Formula: RGB' = RGB * 255 / alpha
     * Saturates on overflow, returns 0 if alpha is 0.
     */
    static inline uint32_t unscale(uint32_t pixel) {
        unsigned int alpha = (pixel >> 24) & 0xFF;

        if (!alpha) {
            return 0;
        }

        // Unscale red component
        unsigned int r = (((pixel & 0xFF0000) * 0xFF) + (((alpha + 1) << 15))) / alpha;

        // Unscale green component
        unsigned int g = (((pixel & 0xFF00) * 0xFF) + (((alpha + 1) << 7))) / alpha;

        // Unscale blue component
        unsigned int b = (((pixel & 0xFF) * 0xFF) + ((alpha + 1) >> 1)) / alpha;

        // Detect overflow (carry) in each component
        uint32_t carry = ((r & 0xFF000000U) | (g & 0xFF0000) | (b & 0xFF00)) >> 8;

        // Propagate carry bits to create saturation mask
        carry = carry | (((carry & 0xAAAAAA) >> 1) | ((carry & 0x555555) << 1));
        carry = carry | (((carry & 0xCCCCCC) >> 2) | ((carry & 0x333333) << 2));
        carry = carry | (((carry & 0xF0F0F0) >> 4) | ((carry & 0xF0F0F) << 4));

        // Extract RGB components
        uint32_t rgb = ((r & 0xFF0000) | (g & 0xFF00)) | (b & 0xFF);

        // Saturate components if division overflows
        rgb = rgb | carry;

        // Preserve alpha channel
        return rgb | (pixel & 0xFF000000U);
    }

    /**
     * Blend two pixels with alpha, assuming colors are pre-scaled.
     * Both source and destination alpha are considered.
     * Formula: dest' = src + dest * (1 - srcAlpha)
     */
    static inline uint32_t blendScaled(uint32_t src, uint32_t dest) {
        // High 8 bits of source pixel is source opacity (ARGB format)
        unsigned int unAlpha = 0xFF - (src >> 24);

        // Blend red and blue components with rounding
        uint32_t rb = ((dest & 0xFF00FF) * unAlpha) + 0x800080;

        // Blend alpha and green components with rounding
        uint32_t ag = (((dest >> 8) & 0xFF00FF) * unAlpha) + 0x800080;

        // Divide by 255
        rb = (((rb >> 8) & 0xFF00FF) + rb) >> 8;
        ag = (((ag >> 8) & 0xFF00FF) + ag) >> 8;

        // Add source components
        rb = (rb & 0xFF00FF) + (src & 0xFF00FF);
        ag = (ag & 0xFF00FF) + ((src >> 8) & 0xFF00FF);

        // Saturate red and blue if carry occurred
        rb = (rb & 0xFF00FF) | (((rb & 0x1000100) * 0xFF) >> 8);

        // Saturate alpha and green if carry occurred
        ag = ((ag & 0xFF00FF) << 8) | ((ag & 0x1000100) * 0xFF);

        return ag | rb;
    }

    /**
     * Blend two pixels with alpha, assuming colors are NOT pre-scaled.
     * Performs full Porter-Duff "over" compositing.
     * Formula: result = (srcAlpha*srcColor + (resultAlpha - srcAlpha)*destColor) / resultAlpha
     *          where resultAlpha = srcAlpha + destAlpha * (1 - srcAlpha)
     */
    static inline uint32_t blendUnscaled(uint32_t src, uint32_t dest) {
        unsigned int alpha = src >> 24;

        if (!alpha) {
            return dest;
        }
        if (alpha == 0xFF) {
            return src;
        }

        // Blend alpha channels: resultAlpha = srcAlpha + destAlpha * (1 - srcAlpha)
        unsigned int blendA = ((0xFF * alpha) +
                              ((0xFF - alpha) * (dest >> 24))) + 128;

        // Divide by 255
        blendA = (((blendA >> 8) + blendA) >> 8) & 0xFF;

        // Blend each color component
        // Formula: (srcColor * srcAlpha + destColor * (blendA - srcAlpha)) / blendA
        unsigned int blendR = (((src & 0xFF0000) * alpha) +
                               ((dest & 0xFF0000) * (blendA - alpha)) +
                               (blendA << 15)) / blendA;
        blendR &= 0xFF0000;

        unsigned int blendG = (((src & 0xFF00) * alpha) +
                               ((dest & 0xFF00) * (blendA - alpha)) +
                               (blendA << 7)) / blendA;
        blendG &= 0xFF00;

        unsigned int blendB = (((src & 0xFF) * alpha) +
                               ((dest & 0xFF) * (blendA - alpha)) +
                               (blendA >> 1)) / blendA;
        blendB &= 0xFF;

        return ((blendR | blendB) | blendG) | (blendA << 24);
    }
};

// ============================================================================
// Alpha Blending Combination Rules
// ============================================================================

/**
 * Standard alpha blend using source pixel's alpha channel.
 * Note: This rule needs special handling for 32bpp only.
 */
template<>
struct CombinationRule<CR_alphaBlend> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        unsigned int alpha = AlphaBlendHelper::extractAlpha(src);
        return AlphaBlendHelper::blend(src, dest, alpha);
    }
};

/**
 * Alpha blend with constant alpha from operation parameters.
 * Note: Requires access to operation_t for sourceAlpha.
 */
template<>
struct CombinationRule<CR_alphaBlendConst> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        // This is a simplified version - actual implementation needs sourceAlpha
        // Will be handled specially in BitBltOperation specialization
        unsigned int alpha = AlphaBlendHelper::extractAlpha(src);
        return AlphaBlendHelper::blend(src, dest, alpha);
    }
};

// ============================================================================
// RGB Color Operations
// ============================================================================

template<>
struct CombinationRule<CR_rgbAdd> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return RGBHelper::add(src, dest);
    }
};

template<>
struct CombinationRule<CR_rgbSub> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return RGBHelper::sub(src, dest);
    }
};

template<>
struct CombinationRule<CR_rgbMul> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return RGBHelper::mul(src, dest);
    }
};

template<>
struct CombinationRule<CR_rgbMax> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return RGBHelper::max(src, dest);
    }
};

template<>
struct CombinationRule<CR_rgbMin> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return RGBHelper::min(src, dest);
    }
};

template<>
struct CombinationRule<CR_rgbMinInvert> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        // Min then invert the result
        uint32_t minResult = RGBHelper::min(src, dest);
        uint8_t a, r, g, b;
        RGBHelper::extract(minResult, a, r, g, b);
        return RGBHelper::combine(a, 255 - r, 255 - g, 255 - b);
    }
};

// ============================================================================
// Advanced Alpha Operations
// ============================================================================

/**
 * Alpha scale (premultiply): Scale RGB by alpha channel.
 * Formula: RGB' = RGB * alpha / 255
 * Used to convert from unscaled to pre-multiplied alpha format.
 */
template<>
struct CombinationRule<CR_alphaScale> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        (void)src;  // Source is ignored
        return AlphaScaleHelper::scale(dest);
    }
};

/**
 * Alpha unscale: Divide RGB by alpha channel.
 * Formula: RGB' = RGB * 255 / alpha
 * Used to convert from pre-multiplied to unscaled alpha format.
 */
template<>
struct CombinationRule<CR_alphaUnscale> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        (void)src;  // Source is ignored
        return AlphaScaleHelper::unscale(dest);
    }
};

/**
 * Alpha blend scaled: Blend assuming colors are pre-scaled by alpha.
 * Formula: dest' = src + dest * (1 - srcAlpha)
 * Both source and destination alpha channels are considered.
 */
template<>
struct CombinationRule<CR_alphaBlendScaled> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return AlphaScaleHelper::blendScaled(src, dest);
    }
};

/**
 * Alpha blend unscaled: Full Porter-Duff "over" compositing.
 * Assumes colors are NOT pre-scaled by alpha.
 * Formula: result = (srcAlpha*srcColor + (resultAlpha - srcAlpha)*destColor) / resultAlpha
 *          where resultAlpha = srcAlpha + destAlpha * (1 - srcAlpha)
 */
template<>
struct CombinationRule<CR_alphaBlendUnscaled> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        return AlphaScaleHelper::blendUnscaled(src, dest);
    }
};

/**
 * Alpha paint const: Paint mode blending with constant alpha.
 * Blends source with destination using a constant alpha value from operation.
 * Note: This requires access to operation_t for sourceAlpha parameter.
 */
template<>
struct CombinationRule<CR_alphaPaintConst> {
    static inline uint32_t apply(uint32_t src, uint32_t dest, unsigned int sourceAlpha) {
        if (!src) {
            return dest;
        }

        // Use same algorithm as alphaBlendConst but in paint mode
        // In paint mode, source color is assumed to be pre-scaled
        unsigned int unAlpha = 0xFF - sourceAlpha;

        // Blend red and blue components
        uint32_t blendRB = ((src & 0xFF00FF) * sourceAlpha) +
                           ((dest & 0xFF00FF) * unAlpha) + 0x800080;

        // Blend alpha and green components
        uint32_t blendAG = (((src >> 8) & 0xFF00FF) * sourceAlpha) +
                           (((dest >> 8) & 0xFF00FF) * unAlpha) + 0x800080;

        // Divide by 255
        blendRB = (((blendRB >> 8) & 0xFF00FF) + blendRB) >> 8;
        blendAG = (((blendAG >> 8) & 0xFF00FF) + blendAG) >> 8;

        blendRB &= 0xFF00FF;
        blendAG &= 0xFF00FF;

        return blendRB | (blendAG << 8);
    }
};

/**
 * Fix alpha: Copy source alpha to destination if destination alpha is 0.
 * Used to fix images that have RGB data but missing alpha channel.
 * Only works with 32bpp pixels.
 */
template<>
struct CombinationRule<CR_fixAlpha> {
    static inline uint32_t apply(uint32_t src, uint32_t dest) {
        // If destination is completely transparent (0), return 0
        if (!dest) {
            return 0;
        }

        // If destination already has alpha, keep it unchanged
        if (dest & 0xFF000000U) {
            return dest;
        }

        // Copy source alpha to destination
        return dest | (src & 0xFF000000U);
    }
};

// ============================================================================
// Specialized Operation Helpers
// ============================================================================

template<unsigned BPP>
class BitBltClearOperation {
public:
    using Traits = PixelFormatTraits<BPP>;
    using Storage = typename Traits::StorageType;
    using Accessor = PixelAccessor<BPP>;

    static void execute(const operation_t* op) {
        auto destBits = static_cast<Storage*>(op->dest.bits);
        const uint32_t destPitch = op->dest.pitch / sizeof(Storage);
        const bool destMSB = op->dest.msb;
        const uint32_t destX = op->dest.x;
        const uint32_t destY = op->dest.y;
        const uint32_t width = op->width;
        const uint32_t height = op->height;

        for (uint32_t y = 0; y < height; ++y) {
            auto rowPtr = destBits + (destY + y) * destPitch;
            if constexpr (BPP >= 8) {
                Storage* line = rowPtr + destX;
                if constexpr (std::is_same_v<Storage, uint8_t>) {
                    std::memset(line, 0, width * sizeof(Storage));
                } else {
                    std::fill(line, line + width, static_cast<Storage>(0));
                }
            } else {
                for (uint32_t x = 0; x < width; ++x) {
                    Accessor::write(rowPtr, destX + x, 0u, destMSB);
                }
            }
        }
    }
};

template<unsigned BPP>
class BitBltPixSwapOperation {
public:
    using Traits = PixelFormatTraits<BPP>;
    using Storage = typename Traits::StorageType;
    using Accessor = PixelAccessor<BPP>;

    static void execute(const operation_t* op) {
        constexpr unsigned PPW = Traits::PIXELS_PER_WORD;
        if constexpr (PPW <= 1) {
            return;
        }

        auto destBits = static_cast<Storage*>(op->dest.bits);
        const uint32_t destPitch = op->dest.pitch / sizeof(Storage);
        const bool destMSB = op->dest.msb;
        const uint32_t destX = op->dest.x;
        const uint32_t destY = op->dest.y;
        const uint32_t width = op->width;
        const uint32_t height = op->height;

        for (uint32_t row = 0; row < height; ++row) {
            auto rowPtr = destBits + (destY + row) * destPitch;
            uint32_t processed = 0;
            while (processed < width) {
                uint32_t globalIndex = destX + processed;
                uint32_t wordBase = (globalIndex / PPW) * PPW;
                uint32_t offset = globalIndex % PPW;
                uint32_t remaining = std::min(PPW - offset, width - processed);

                std::array<uint32_t, PPW> original;
                for (unsigned idx = 0; idx < PPW; ++idx) {
                    uint32_t pixelIndex = wordBase + idx;
                    if constexpr (BPP < 8) {
                        original[idx] = Accessor::read(rowPtr, pixelIndex, destMSB);
                    } else {
                        original[idx] = Accessor::read(rowPtr, pixelIndex);
                    }
                }

                for (uint32_t i = 0; i < remaining; ++i) {
                    uint32_t pixelIndex = wordBase + offset + i;
                    uint32_t swapIndex = PPW - 1 - (offset + i);
                    uint32_t value = original[swapIndex];
                    if constexpr (BPP < 8) {
                        Accessor::write(rowPtr, pixelIndex, value, destMSB);
                    } else {
                        Accessor::write(rowPtr, pixelIndex, value);
                    }
                }

                processed += remaining;
            }
        }
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
// Specialized Clear Operations
// ============================================================================

template<>
class BitBltOperation<1, 1, CR_clearWord> {
public:
    static void execute(const operation_t* op) {
        BitBltClearOperation<1>::execute(op);
    }
};

template<>
class BitBltOperation<2, 2, CR_clearWord> {
public:
    static void execute(const operation_t* op) {
        BitBltClearOperation<2>::execute(op);
    }
};

template<>
class BitBltOperation<4, 4, CR_clearWord> {
public:
    static void execute(const operation_t* op) {
        BitBltClearOperation<4>::execute(op);
    }
};

template<>
class BitBltOperation<8, 8, CR_clearWord> {
public:
    static void execute(const operation_t* op) {
        BitBltClearOperation<8>::execute(op);
    }
};

template<>
class BitBltOperation<16, 16, CR_clearWord> {
public:
    static void execute(const operation_t* op) {
        BitBltClearOperation<16>::execute(op);
    }
};

// ============================================================================
// Specialized PixSwap Operations
// ============================================================================

template<>
class BitBltOperation<1, 1, CR_pixSwap> {
public:
    static void execute(const operation_t* op) {
        BitBltPixSwapOperation<1>::execute(op);
    }
};

template<>
class BitBltOperation<2, 2, CR_pixSwap> {
public:
    static void execute(const operation_t* op) {
        BitBltPixSwapOperation<2>::execute(op);
    }
};

template<>
class BitBltOperation<4, 4, CR_pixSwap> {
public:
    static void execute(const operation_t* op) {
        BitBltPixSwapOperation<4>::execute(op);
    }
};

template<>
class BitBltOperation<8, 8, CR_pixSwap> {
public:
    static void execute(const operation_t* op) {
        BitBltPixSwapOperation<8>::execute(op);
    }
};

template<>
class BitBltOperation<16, 16, CR_pixSwap> {
public:
    static void execute(const operation_t* op) {
        BitBltPixSwapOperation<16>::execute(op);
    }
};

template<>
class BitBltOperation<32, 32, CR_pixSwap> {
public:
    static void execute(const operation_t* op) {
        BitBltPixSwapOperation<32>::execute(op);
    }
};

// ============================================================================
// Specialized RGB Utility Operations
// ============================================================================

template<>
class BitBltOperation<32, 32, CR_rgbDiff> {
public:
    static void execute(const operation_t* op) {
        auto srcBits = static_cast<const uint32_t*>(op->src.bits);
        auto destBits = static_cast<const uint32_t*>(op->dest.bits);

        const uint32_t srcPitch = op->src.pitch / sizeof(uint32_t);
        const uint32_t destPitch = op->dest.pitch / sizeof(uint32_t);
        const uint32_t srcX = op->src.x;
        const uint32_t srcY = op->src.y;
        const uint32_t destX = op->dest.x;
        const uint32_t destY = op->dest.y;
        const uint32_t width = op->width;
        const uint32_t height = op->height;

        int64_t localCount = 0;

        for (uint32_t y = 0; y < height; ++y) {
            const uint32_t* srcRow = srcBits + (srcY + y) * srcPitch;
            const uint32_t* destRow = destBits + (destY + y) * destPitch;
            for (uint32_t x = 0; x < width; ++x) {
                uint32_t srcPixel = srcRow[srcX + x];
                uint32_t destPixel = destRow[destX + x];
                uint32_t diff = CombinationRule<CR_rgbDiff>::difference<32>(srcPixel, destPixel);
                localCount += static_cast<int64_t>(diff);
            }
        }

        if (op->opt.tally.bitCount) {
            *(op->opt.tally.bitCount) += localCount;
        }
    }
};

template<>
class BitBltOperation<32, 32, CR_rgbComponentAlpha> {
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

        ComponentAlphaParams params{
            static_cast<uint32_t>(op->opt.componentAlpha.componentAlphaModeColor),
            static_cast<uint8_t>(op->opt.componentAlpha.componentAlphaModeAlpha & 0xFF),
            op->opt.componentAlpha.gammaLookupTable,
            op->opt.componentAlpha.ungammaLookupTable
        };

        for (uint32_t y = 0; y < height; ++y) {
            const uint32_t* srcRow = srcBits + (srcY + y) * srcPitch;
            uint32_t* destRow = destBits + (destY + y) * destPitch;
            for (uint32_t x = 0; x < width; ++x) {
                uint32_t srcPixel = srcRow[srcX + x];
                uint32_t destPixel = destRow[destX + x];
                uint32_t result = CombinationRule<CR_rgbComponentAlpha>::apply(srcPixel, destPixel, params);
                destRow[destX + x] = result;
            }
        }
    }
};

/**
 * Specialized operation for alpha paint const that accesses sourceAlpha from operation.
 */
template<>
class BitBltOperation<32, 32, CR_alphaPaintConst> {
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

        // Extract sourceAlpha from operation
        const unsigned int sourceAlpha = static_cast<unsigned int>(op->opt.sourceAlpha) & 0xFF;

        for (uint32_t y = 0; y < height; ++y) {
            const uint32_t* srcRow = srcBits + (srcY + y) * srcPitch;
            uint32_t* destRow = destBits + (destY + y) * destPitch;
            for (uint32_t x = 0; x < width; ++x) {
                uint32_t srcPixel = srcRow[srcX + x];
                uint32_t destPixel = destRow[destX + x];
                uint32_t result = CombinationRule<CR_alphaPaintConst>::apply(srcPixel, destPixel, sourceAlpha);
                destRow[destX + x] = result;
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

// 32bpp -> 16bpp operations
void bitblt_32_16_sourceWord(operation_t* op, uint32_t flags);

// 32bpp -> 8bpp operations
void bitblt_32_8_sourceWord(operation_t* op, uint32_t flags);

// 16bpp -> 16bpp operations
void bitblt_16_16_sourceWord(operation_t* op, uint32_t flags);
void bitblt_16_16_clearWord(operation_t* op, uint32_t flags);

// 8bpp -> 8bpp operations
void bitblt_8_8_sourceWord(operation_t* op, uint32_t flags);
void bitblt_8_8_clearWord(operation_t* op, uint32_t flags);

// 4bpp -> 4bpp operations
void bitblt_4_4_clearWord(operation_t* op, uint32_t flags);

// 2bpp -> 2bpp operations
void bitblt_2_2_clearWord(operation_t* op, uint32_t flags);

// 1bpp -> 1bpp operations
void bitblt_1_1_clearWord(operation_t* op, uint32_t flags);

// Alpha blending operations (32bpp only)
void bitblt_32_32_alphaBlend(operation_t* op, uint32_t flags);
void bitblt_32_32_alphaBlendConst(operation_t* op, uint32_t flags);

// RGB color operations (32bpp)
void bitblt_32_32_rgbAdd(operation_t* op, uint32_t flags);
void bitblt_32_32_rgbSub(operation_t* op, uint32_t flags);
void bitblt_32_32_rgbMul(operation_t* op, uint32_t flags);
void bitblt_32_32_rgbMax(operation_t* op, uint32_t flags);
void bitblt_32_32_rgbMin(operation_t* op, uint32_t flags);
void bitblt_32_32_rgbMinInvert(operation_t* op, uint32_t flags);

// Pixel operations (various depths)
void bitblt_1_1_pixPaint(operation_t* op, uint32_t flags);
void bitblt_2_2_pixPaint(operation_t* op, uint32_t flags);
void bitblt_4_4_pixPaint(operation_t* op, uint32_t flags);
void bitblt_8_8_pixPaint(operation_t* op, uint32_t flags);
void bitblt_16_16_pixPaint(operation_t* op, uint32_t flags);
void bitblt_32_32_pixPaint(operation_t* op, uint32_t flags);

void bitblt_1_1_pixMask(operation_t* op, uint32_t flags);
void bitblt_2_2_pixMask(operation_t* op, uint32_t flags);
void bitblt_4_4_pixMask(operation_t* op, uint32_t flags);
void bitblt_8_8_pixMask(operation_t* op, uint32_t flags);
void bitblt_16_16_pixMask(operation_t* op, uint32_t flags);
void bitblt_32_32_pixMask(operation_t* op, uint32_t flags);

void bitblt_1_1_pixSwap(operation_t* op, uint32_t flags);
void bitblt_2_2_pixSwap(operation_t* op, uint32_t flags);
void bitblt_4_4_pixSwap(operation_t* op, uint32_t flags);
void bitblt_8_8_pixSwap(operation_t* op, uint32_t flags);
void bitblt_16_16_pixSwap(operation_t* op, uint32_t flags);
void bitblt_32_32_pixSwap(operation_t* op, uint32_t flags);

void bitblt_1_1_pixClear(operation_t* op, uint32_t flags);
void bitblt_2_2_pixClear(operation_t* op, uint32_t flags);
void bitblt_4_4_pixClear(operation_t* op, uint32_t flags);
void bitblt_8_8_pixClear(operation_t* op, uint32_t flags);
void bitblt_16_16_pixClear(operation_t* op, uint32_t flags);
void bitblt_32_32_pixClear(operation_t* op, uint32_t flags);

// RGB utilities
void bitblt_32_32_rgbDiff(operation_t* op, uint32_t flags);
void bitblt_32_32_rgbComponentAlpha(operation_t* op, uint32_t flags);

} // extern "C"

} // namespace BitBlt

#endif // BITBLTTEMPLATES_HPP_
