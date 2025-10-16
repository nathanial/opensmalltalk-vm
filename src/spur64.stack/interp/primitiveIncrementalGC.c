/* Extracted from interp.c:68640 (function primitiveIncrementalGC). */

static void
primitiveIncrementalGC(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerVal;
    char *sp;

	/* begin externalWriteBackHeadFramePointers */
	assert((GIV(framePointer) - GIV(stackPointer)) < (LargeContextSlots * BytesPerOop));
	assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
	assert(!((isFree(GIV(stackPage)))));

	/* begin setHeadFP:andSP:inPage: */
	assert(GIV(stackPointer) < GIV(framePointer));
	assert((GIV(stackPointer) < ((GIV(stackPage)->baseAddress)))
	 && (GIV(stackPointer) > (((GIV(stackPage)->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
	assert((GIV(framePointer) < ((GIV(stackPage)->baseAddress)))
	 && (GIV(framePointer) > (((GIV(stackPage)->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
	(GIV(stackPage)->headFP = GIV(framePointer));
	(GIV(stackPage)->headSP = GIV(stackPointer));
	assert(pageListIsWellFormed());

	/* begin scavengingGC */
	scavengingGCTenuringIf(TenureByAge);
	integerVal = ((((GIV(totalFreeOldSpace) + (GIV(scavengeThreshold) - GIV(freeStart))) - (interpreterAllocationReserveBytes())) < 0) ? 0 : ((GIV(totalFreeOldSpace) + (GIV(scavengeThreshold) - GIV(freeStart))) - (interpreterAllocationReserveBytes())));

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer)),(((usqInt)integerVal << 3) | 1));
	GIV(stackPointer) = sp;
}