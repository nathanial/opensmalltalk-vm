/* Extracted from interp.c:52743 (function externalInstVarofContext). */

static NoDbgRegParms sqInt
externalInstVarofContext(sqInt offset, sqInt aContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(isContext(aContext));
	assert(offset <= (ReceiverIndex + (checkStackPointerForMaybeMarriedContext(aContext))));

	/* method, closureOrNil & receiver need no special handling; only
	   sender, pc & stackp have to be computed for married contexts. */
	if (offset <= StackPointerIndex) {
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
		if (/* isStillMarriedContext: */
			(((((longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(aContext)))) {
			return fetchPointerofMarriedContext(offset, aContext);
		}
	}
	return longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(offset) << (shiftForWord()))))));
}