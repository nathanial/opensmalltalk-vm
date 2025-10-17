/* Extracted from interp.c:51800 (function divorceAllFrames). */

	/* StackInterpreter>>#divorceAllFrames */

static sqInt
divorceAllFrames(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeContext;
    StackPage *aPage;
    sqInt i;

	if (stackPage) {
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
	}

	/* begin ensureFrameIsMarried:SP: */
	if (byteAt((framePointer + FoxFrameFlags) + 2)) {
		assert(isContext(frameContext(framePointer)));
		activeContext = longAt(framePointer + FoxThisContext);
		goto l1;
	}
	activeContext = marryFrameSP(framePointer, stackPointer + BytesPerWord);
	/* end ensureFrameIsMarried:SP: */
l1:
	for (i = 0; i < numStackPages; i += 1) {
		/* begin stackPageAt: */
		aPage = stackPageAtpages(i, pages);
		if (!(isFree(aPage))) {
			divorceFramesIn(aPage);
		}
	}

	/* begin nilStackPage */
	assert((!stackPage)
	 || ((((stackPage->headFP)) == framePointer)
	 && (((stackPage->headSP)) == stackPointer)));
	stackPage = null;
	return activeContext;
}