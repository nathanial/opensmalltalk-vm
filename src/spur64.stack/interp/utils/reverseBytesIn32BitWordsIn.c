/* Extracted from interp.c:45224 (function reverseBytesIn32BitWordsIn). */

/*	This exists to get around a compiler bug in Apple LLVM version 7.0.0
        (clang-700.1.76) that was avoiding the second comparison of segVersion
        after the first byte swap. */

/* SpurMemoryManager>>#reverseBytesIn32BitWordsIn: */

static NeverInline void reverseBytesIn32BitWordsIn(sqInt segmentWordArray) {
  sqInt addr;
  sqInt stopAddr;

  stopAddr = addressAfter(segmentWordArray);

  /* begin reverseBytesIn32BitWordsFrom:to: */
  addr = segmentWordArray + BaseHeaderSize;
  while (oopisLessThan(addr, stopAddr)) {
    long32Atput((void *)(addr), SQ_SWAP_4_BYTES((long32At((void *)(addr)))));
    addr += 4;
  }
}