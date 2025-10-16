/* Extracted from interp.c:54633 (function handleStackOverflowOrEventAllowContextSwitch). */

static NoDbgRegParms sqInt
handleStackOverflowOrEventAllowContextSwitch(sqInt mayContextSwitch)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	/* begin checkForStackOverflow */
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
	if (GIV(stackPointer) < ((GIV(stackPage)->realStackLimit))) {
		handleStackOverflow();
	}

	/* If the stackLimit differs from the realStackLimit then the stackLimit
	   has been set to indicate an event or interrupt that needs servicing. */
	if (GIV(stackLimit) == ((GIV(stackPage)->realStackLimit))) {
		return 0;
	}
	return checkForEventsMayContextSwitch(mayContextSwitch);
}