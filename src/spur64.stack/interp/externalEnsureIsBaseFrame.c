/* Extracted from interp.c:52703 (function externalEnsureIsBaseFrame). */

static NoDbgRegParms char *
externalEnsureIsBaseFrame(char *aFramePtr)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int onCurrent;
    char *theFP;
    StackPage *thePage;

	if (!(longAt(aFramePtr + FoxSavedFP))) {
		return aFramePtr;
	}
	theFP = aFramePtr;

	/* begin stackPageFor: */
	thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
	onCurrent = thePage == GIV(stackPage);

	/* Storing the frame's sender with its caller's context
	   has the side effect of making theFP a base frame. */
	theFP = storeSenderOfFramewithValue(theFP, ensureCallerContext(theFP));
	if (onCurrent) {
		assert(GIV(stackPage) != thePage);

		/* begin setStackPointersFromPage: */
		GIV(stackPointer) = (GIV(stackPage)->headSP);
		GIV(framePointer) = (GIV(stackPage)->headFP);
	}
	else {
		markStackPageMostRecentlyUsed(GIV(stackPage));
	}
	assert(pageListIsWellFormed());
	assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
	return theFP;
}