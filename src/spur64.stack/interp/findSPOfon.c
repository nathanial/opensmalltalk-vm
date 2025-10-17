/* Extracted from interp.c:53560 (function findSPOfon). */

/*	Search for the stack pointer for theFP. This points to the hottest item on
	the frame's stack.
	DO NOT CALL THIS WITH theFP == localFP OR theFP == framePointer! */

	/* StackInterpreter>>#findSPOf:on: */

static NoDbgRegParms char *
findSPOfon(char *theFP, StackPage *thePage)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *aFrame;
    char *prevFrame;
    char *startFrame;
    char *theSP;

	assert(!((isFree(thePage))));
	startFrame = (thePage->headFP);

	/* begin findSPOrNilOf:on:startingFrom: */
	prevFrame = ((char *) 0);
	if (startFrame == theFP) {
		if (((thePage->headSP)) >= startFrame) {
			theSP = theFP + FoxReceiver;
			goto l1;
		}

		/* If the SP is invalid return the pointer to the receiver field.
		   Skip the instruction pointer on top of stack of inactive pages. */
		theSP = (thePage == stackPage
					? (thePage->headSP)
					: ((thePage->headSP)) + BytesPerWord);
		goto l1;
	}
	aFrame = startFrame;
	while (1) {
		prevFrame = aFrame;
		aFrame = ((char *)(longAt(aFrame + FoxSavedFP)));
		if (!(aFrame != 0)) break;
		if (theFP == aFrame) {
			/* begin frameCallerSP: */
			assert(!(isBaseFrame(prevFrame)));
			theSP = (prevFrame + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((prevFrame + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord;
			goto l1;
		}
	}
	theSP = null;
	/* end findSPOrNilOf:on:startingFrom: */
l1:
	if (theSP) {
		return theSP;
	}
	error("did not find theFP in stack page");
	return 0;
}