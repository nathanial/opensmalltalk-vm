/* Extracted from interp.c:51800 (function divorceAllFrames). */

static sqInt
divorceAllFrames(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeContext;
    StackPage *aPage;
    sqInt i;

	if (GIV(stackPage)) {
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
	}

	/* begin ensureFrameIsMarried:SP: */
	if (byteAt((GIV(framePointer) + FoxFrameFlags) + 2)) {
		assert(isContext(frameContext(GIV(framePointer))));
		activeContext = longAt(GIV(framePointer) + FoxThisContext);
		goto l1;
	}
	activeContext = marryFrameSP(GIV(framePointer), GIV(stackPointer) + BytesPerWord);
	/* end ensureFrameIsMarried:SP: */
l1:
	for (i = 0; i < GIV(numStackPages); i += 1) {
		/* begin stackPageAt: */
		aPage = stackPageAtpages(i, GIV(pages));
		if (!(isFree(aPage))) {
			divorceFramesIn(aPage);
		}
	}

	/* begin nilStackPage */
	assert((!GIV(stackPage))
	 || ((((GIV(stackPage)->headFP)) == GIV(framePointer))
	 && (((GIV(stackPage)->headSP)) == GIV(stackPointer))));
	GIV(stackPage) = null;
	return activeContext;
}