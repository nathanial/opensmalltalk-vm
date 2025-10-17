/* Extracted from interp.c:51376 (function checkStackPointerIndexForFrame). */

/*	Version of stackPointerIndexForFrame: that does not depend on writing back
	head frame pointers.
	Used for assertion checking. Safe only in external primitives
	(framePointer valid).
	Answer the 0-based index rel to the given frame.
	(This is what stackPointer used to be before conversion to pointer) */
/*	In the StackInterpreter stacks grow down. */

	/* StackInterpreter>>#checkStackPointerIndexForFrame: */

static NoDbgRegParms sqInt
checkStackPointerIndexForFrame(char *theFP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *aFrame;
    char *prevFrame;
    char *startFrame;
    StackPage *thePage;
    char *theSP;

	if (theFP == framePointer) {
		return (((usqInt)(((theFP + FoxReceiver) - stackPointer))) >> (shiftForWord())) + (byteAt((theFP + FoxFrameFlags) + 1));
	}

	/* begin stackPageFor: */
	thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage), pages);
	startFrame = (thePage == stackPage
				? framePointer
				: (thePage->headFP));

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
	if (!theSP) {
		return -1;
	}
	return (((usqInt)(((theFP + FoxReceiver) - theSP))) >> (shiftForWord())) + (byteAt((theFP + FoxFrameFlags) + 1));
}