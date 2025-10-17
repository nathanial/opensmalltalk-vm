/* Extracted from interp.c:54532 (function handleStackOverflow). */

/*	Check for stack overflow, moving frames to another stack if so.
	This should *only* be sent from checkForStackOverflow. */

	/* StackInterpreter>>#handleStackOverflow */

static NeverInline void
handleStackOverflow(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *callerFP;
    char *callerFP1;
    StackPage *newPage;
    sqInt overflowCount;
    char *overflowLimitAddress;
    char *theFP;

	callerFP = ((char *) 0);
	assert(GIV(stackPointer) < ((GIV(stackPage)->realStackLimit)));

	/* begin traceStackOverflow */
	/* begin recordTrace:thing:source: */
	if (TraceLog) {
		GIV(traceLog)[GIV(traceLogIndex)] = TraceStackOverflow;
		GIV(traceLog)[GIV(traceLogIndex) + 1] = TraceStackOverflow;
		GIV(traceLog)[GIV(traceLogIndex) + 2] = 0;
		GIV(traceLogIndex) = (GIV(traceLogIndex) + 3) % TraceBufferSize;
	}
	GIV(statStackOverflow) += 1;

	/* The stack has overflowed this page.  If the system is executing some recursive algorithm,
	   e.g. fibonacci, then the system could thrash overflowing the stack if the call soon returns
	   back to the current page.  To avoid thrashing, since overflow is quite slow, we can move
	   more than one frame.  The idea is to record which page has overflowed, and the first
	   time it overflows move one frame, the second time two frames, and so on.  We move no
	   more frames than would leave the page half occupied. */
	theFP = GIV(framePointer);
	if (GIV(stackPage) == GIV(overflowedPage)) {
		overflowLimitAddress = ((GIV(stackPage)->baseAddress)) - GIV(overflowLimit);
		overflowCount = (GIV(extraFramesToMoveOnOverflow) += 1);
		while ((((overflowCount -= 1)) >= 0)
		 && ((((callerFP = ((char *)(longAt(theFP + FoxSavedFP))))) < overflowLimitAddress)
		 && (!((longAt(callerFP + FoxSavedFP)) == 0)))) {
			theFP = callerFP;
		}
	}
	else {
		GIV(overflowedPage) = GIV(stackPage);
		GIV(extraFramesToMoveOnOverflow) = 0;
	}

	/* begin ensureCallerContext: */
	callerFP1 = ((char *)(longAt(theFP + FoxSavedFP)));
	if (!callerFP1) {
		/* begin frameCallerContext: */
		assert(isBaseFrame(theFP));
		goto l2;
	}

	/* base frame, context in saved ip slot (or base of stack in Cog) */

	/* begin ensureFrameIsMarried:SP: */
	if (byteAt((callerFP1 + FoxFrameFlags) + 2)) {
		assert(isContext(frameContext(callerFP1)));

		/* frameContext: */
		goto l2;
	}
	marryFrameSP(
		callerFP1,
		(/* begin frameCallerStackPointer: */
			assert(!(isBaseFrame(theFP))),
		(theFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord));
	/* end ensureCallerContext: */
l2:

	/* begin newStackPage */
	newPage = (GIV(mostRecentlyUsedPage)->nextPage);
	if (!((newPage->baseFP))) {
		goto l1;
	}
	divorceFramesIn(newPage);
	/* end newStackPage */
l1:
	moveFramesInthroughtoPage(GIV(stackPage), theFP, newPage);

	/* begin setStackPageAndLimit: */
	assert(newPage);
	GIV(stackPage) = newPage;
	if (GIV(stackLimit) != (((char *) (((usqInt) -1))))) {
		GIV(stackLimit) = (GIV(stackPage)->stackLimit);
	}
	markStackPageMostRecentlyUsed(newPage);

	/* begin setStackPointersFromPage: */
	GIV(stackPointer) = (newPage->headSP);
	GIV(framePointer) = (newPage->headFP);
	assert(!(frameHasContext(GIV(framePointer))));
	assert(validInstructionPointerinMethodframePointer(GIV(instructionPointer) + 1, GIV(method), GIV(framePointer)));
}