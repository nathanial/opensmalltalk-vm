/* Extracted from interp.c:68640 (function primitiveIncrementalGC). */

/*	Do a quick, incremental garbage collection and return the number of bytes
	immediately available.
	(Note: more space may be made available by doing a full garbage
	collection. 
 */
/*	Do a quick, incremental garbage collection and return the number of bytes
	immediately available.
	(Note: more space may be made available by doing a full garbage
	collection. 
 */

	/* StackInterpreterPrimitives>>#primitiveIncrementalGC */

static void
primitiveIncrementalGC(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt integerVal;
    char *sp;

	/* begin externalWriteBackHeadFramePointers */
	assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
	assert(stackPage == (mostRecentlyUsedPage));
	assert(!((isFree(stackPage))));

	/* begin setHeadFP:andSP:inPage: */
	assert(stackPointer < framePointer);
	assert((stackPointer < ((stackPage->baseAddress)))
	 && (stackPointer > (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
	assert((framePointer < ((stackPage->baseAddress)))
	 && (framePointer > (((stackPage->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
	(stackPage->headFP = framePointer);
	(stackPage->headSP = stackPointer);
	assert(pageListIsWellFormed());

	/* begin scavengingGC */
	scavengingGCTenuringIf(TenureByAge);
	integerVal = ((((totalFreeOldSpace + (scavengeThreshold - freeStart)) - (interpreterAllocationReserveBytes())) < 0) ? 0 : ((totalFreeOldSpace + (scavengeThreshold - freeStart)) - (interpreterAllocationReserveBytes())));

	/* begin pop:thenPushInteger: */
	longAtput((sp = stackPointer),(((usqInt)integerVal << 3) | 1));
	stackPointer = sp;
}