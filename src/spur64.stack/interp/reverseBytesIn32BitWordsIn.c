/* Extracted from interp.c:45224 (function reverseBytesIn32BitWordsIn). */

static NoDbgRegParms NeverInline void
reverseBytesIn32BitWordsIn(sqInt segmentWordArray)
{
    sqInt addr;
    sqInt stopAddr;

	stopAddr = addressAfter(segmentWordArray);

	/* begin reverseBytesIn32BitWordsFrom:to: */
	addr = segmentWordArray + BaseHeaderSize;
	while (oopisLessThan(addr, stopAddr)) {
		long32Atput((void *)(addr),SQ_SWAP_4_BYTES((long32At((void *)(addr)))));
		addr += 4;
	}
}