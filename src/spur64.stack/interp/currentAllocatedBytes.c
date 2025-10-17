/* Extracted from interp.c:34107 (function currentAllocatedBytes). */

/*	Compute the current allocated bytes since last set.
	This is the cumulative total in statAllocatedBytes plus the allocation
	since the last scavenge. */
/*	Slang infers the type of the difference between two unsigned variables as
	signed. In this case we want it to be unsigned. */

	/* SpurMemoryManager>>#currentAllocatedBytes */

static usqLong
currentAllocatedBytes(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt use;

	use = ((/* begin totalOldSpaceCapacity */
	assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
totalHeapSizeIncludingBridges - (numSegments * (2 * BaseHeaderSize)))) - totalFreeOldSpace;
	return (statAllocatedBytes + (freeStart - (((eden).start)))) + (use - oldSpaceUsePriorToScavenge);
}