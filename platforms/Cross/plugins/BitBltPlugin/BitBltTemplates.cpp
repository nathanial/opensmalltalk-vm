/*
 * BitBltTemplates.cpp - Template instantiations and C API bridge
 *
 * This file provides the C-compatible wrapper functions that bridge between
 * the C API and the C++ template implementations.
 *
 * Copyright © 2025 OpenSmalltalk VM
 */

#include "BitBltTemplates.hpp"
#include "BitBltInternal.h"

namespace BitBlt {

// ============================================================================
// C API Bridge Implementation
// ============================================================================

extern "C" {

// 32bpp -> 32bpp operations
void bitblt_32_32_clearWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_clearWord>::execute(op);
}

void bitblt_32_32_sourceWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_sourceWord>::execute(op);
}

void bitblt_32_32_bitAnd(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitAnd>::execute(op);
}

void bitblt_32_32_bitOr(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitOr>::execute(op);
}

void bitblt_32_32_bitXor(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitXor>::execute(op);
}

void bitblt_32_32_bitInvertAnd(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitInvertAnd>::execute(op);
}

void bitblt_32_32_bitAndInvert(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitAndInvert>::execute(op);
}

void bitblt_32_32_destinationWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_destinationWord>::execute(op);
}

void bitblt_32_32_bitInvertXor(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitInvertXor>::execute(op);
}

void bitblt_32_32_bitInvertDestination(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitInvertDestination>::execute(op);
}

void bitblt_32_32_bitOrInvert(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitOrInvert>::execute(op);
}

void bitblt_32_32_bitInvertSource(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitInvertSource>::execute(op);
}

void bitblt_32_32_bitInvertOr(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitInvertOr>::execute(op);
}

void bitblt_32_32_bitInvertOrInvert(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_bitInvertOrInvert>::execute(op);
}

void bitblt_32_32_addWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_addWord>::execute(op);
}

void bitblt_32_32_subWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_subWord>::execute(op);
}

// 8bpp -> 32bpp operations
void bitblt_8_32_sourceWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<8, 32, CR_sourceWord>::execute(op);
}

void bitblt_8_32_bitAnd(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<8, 32, CR_bitAnd>::execute(op);
}

void bitblt_8_32_bitOr(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<8, 32, CR_bitOr>::execute(op);
}

void bitblt_8_32_bitXor(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<8, 32, CR_bitXor>::execute(op);
}

// 16bpp -> 32bpp operations
void bitblt_16_32_sourceWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<16, 32, CR_sourceWord>::execute(op);
}

void bitblt_16_32_bitAnd(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<16, 32, CR_bitAnd>::execute(op);
}

void bitblt_16_32_bitOr(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<16, 32, CR_bitOr>::execute(op);
}

void bitblt_16_32_bitXor(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<16, 32, CR_bitXor>::execute(op);
}

// 32bpp -> 16bpp operations
void bitblt_32_16_sourceWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 16, CR_sourceWord>::execute(op);
}

// 32bpp -> 8bpp operations
void bitblt_32_8_sourceWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 8, CR_sourceWord>::execute(op);
}

// 16bpp -> 16bpp operations
void bitblt_16_16_sourceWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<16, 16, CR_sourceWord>::execute(op);
}

void bitblt_16_16_clearWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<16, 16, CR_clearWord>::execute(op);
}

// 8bpp -> 8bpp operations
void bitblt_8_8_sourceWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<8, 8, CR_sourceWord>::execute(op);
}

void bitblt_8_8_clearWord(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<8, 8, CR_clearWord>::execute(op);
}

// ============================================================================
// Alpha Blending Operations (32bpp only)
// ============================================================================

void bitblt_32_32_alphaBlend(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_alphaBlend>::execute(op);
}

void bitblt_32_32_alphaBlendConst(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_alphaBlendConst>::execute(op);
}

// ============================================================================
// RGB Color Operations (32bpp)
// ============================================================================

void bitblt_32_32_rgbAdd(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_rgbAdd>::execute(op);
}

void bitblt_32_32_rgbSub(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_rgbSub>::execute(op);
}

void bitblt_32_32_rgbMul(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_rgbMul>::execute(op);
}

void bitblt_32_32_rgbMax(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_rgbMax>::execute(op);
}

void bitblt_32_32_rgbMin(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_rgbMin>::execute(op);
}

void bitblt_32_32_rgbMinInvert(operation_t* op, uint32_t flags) {
    (void)flags;
    BitBltOperation<32, 32, CR_rgbMinInvert>::execute(op);
}

} // extern "C"

} // namespace BitBlt

// ============================================================================
// Fast Path Registration
// ============================================================================

extern "C" {

/**
 * Register all template-based fast paths with the dispatcher.
 * This function should be called from initialiseCopyBits().
 */
void registerTemplateFastPaths(void) {
    // Array of template-based fast paths
    fast_path_t templatePaths[] = {
        // 32bpp -> 32bpp operations
        { BitBlt::bitblt_32_32_clearWord,      CR_clearWord,      STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_sourceWord,     CR_sourceWord,     STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_bitAnd,         CR_bitAnd,         STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_bitOr,          CR_bitOr,          STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_bitXor,         CR_bitXor,         STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_bitInvertAnd,   CR_bitInvertAnd,   STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_bitAndInvert,   CR_bitAndInvert,   STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_destinationWord,CR_destinationWord,STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_bitInvertXor,   CR_bitInvertXor,   STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_addWord,        CR_addWord,        STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_subWord,        CR_subWord,        STD_FLAGS(32, 32, NO, NO) },

        // 8bpp -> 32bpp operations (with color map support could be added)
        { BitBlt::bitblt_8_32_sourceWord,      CR_sourceWord,     STD_FLAGS(8, 32, DIRECT, NO) },
        { BitBlt::bitblt_8_32_bitAnd,          CR_bitAnd,         STD_FLAGS(8, 32, DIRECT, NO) },
        { BitBlt::bitblt_8_32_bitOr,           CR_bitOr,          STD_FLAGS(8, 32, DIRECT, NO) },
        { BitBlt::bitblt_8_32_bitXor,          CR_bitXor,         STD_FLAGS(8, 32, DIRECT, NO) },

        // 16bpp -> 32bpp operations
        { BitBlt::bitblt_16_32_sourceWord,     CR_sourceWord,     STD_FLAGS(16, 32, DIRECT, NO) },
        { BitBlt::bitblt_16_32_bitAnd,         CR_bitAnd,         STD_FLAGS(16, 32, DIRECT, NO) },
        { BitBlt::bitblt_16_32_bitOr,          CR_bitOr,          STD_FLAGS(16, 32, DIRECT, NO) },
        { BitBlt::bitblt_16_32_bitXor,         CR_bitXor,         STD_FLAGS(16, 32, DIRECT, NO) },

        // 32bpp -> 16bpp operations
        { BitBlt::bitblt_32_16_sourceWord,     CR_sourceWord,     STD_FLAGS(32, 16, DIRECT, NO) },

        // 32bpp -> 8bpp operations
        { BitBlt::bitblt_32_8_sourceWord,      CR_sourceWord,     STD_FLAGS(32, 8, DIRECT, NO) },

        // 16bpp -> 16bpp operations
        { BitBlt::bitblt_16_16_sourceWord,     CR_sourceWord,     STD_FLAGS(16, 16, NO, NO) },
        { BitBlt::bitblt_16_16_clearWord,      CR_clearWord,      STD_FLAGS(16, 16, NO, NO) },

        // 8bpp -> 8bpp operations
        { BitBlt::bitblt_8_8_sourceWord,       CR_sourceWord,     STD_FLAGS(8, 8, NO, NO) },
        { BitBlt::bitblt_8_8_clearWord,        CR_clearWord,      STD_FLAGS(8, 8, NO, NO) },

        // Alpha blending operations (32bpp -> 32bpp only)
        { BitBlt::bitblt_32_32_alphaBlend,     CR_alphaBlend,     STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_alphaBlendConst, CR_alphaBlendConst, STD_FLAGS(32, 32, NO, NO) },

        // RGB color operations (32bpp -> 32bpp)
        { BitBlt::bitblt_32_32_rgbAdd,         CR_rgbAdd,         STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_rgbSub,         CR_rgbSub,         STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_rgbMul,         CR_rgbMul,         STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_rgbMax,         CR_rgbMax,         STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_rgbMin,         CR_rgbMin,         STD_FLAGS(32, 32, NO, NO) },
        { BitBlt::bitblt_32_32_rgbMinInvert,   CR_rgbMinInvert,   STD_FLAGS(32, 32, NO, NO) },
    };

    addFastPaths(templatePaths, sizeof(templatePaths) / sizeof(templatePaths[0]));
}

} // extern "C"
