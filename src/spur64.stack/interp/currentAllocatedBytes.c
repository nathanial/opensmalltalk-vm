/* Extracted from interp.c:34107 (function currentAllocatedBytes). */

static usqLong
currentAllocatedBytes(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt use;

	use = ((/* begin totalOldSpaceCapacity */
	assert((totalBytesInSegments()) == GIV(totalHeapSizeIncludingBridges)),
GIV(totalHeapSizeIncludingBridges) - (GIV(numSegments) * (2 * BaseHeaderSize)))) - GIV(totalFreeOldSpace);
	return (GIV(statAllocatedBytes) + (GIV(freeStart) - (((GIV(eden)).start)))) + (use - GIV(oldSpaceUsePriorToScavenge));
}