/* Extracted from interp.c:54532 (function handleStackOverflow). */

/*	Check for stack overflow, moving frames to another stack if so.
	This should *only* be sent from checkForStackOverflow. */

	/* StackInterpreter>>#handleStackOverflow */

static NeverInline void
handleStackOverflow(void)
{
    char *callerFP;
    char *callerFP1;
    StackPage *newPage;
    sqInt overflowCount;
    char *overflowLimitAddress;
    char *theFP;

	callerFP = ((char *) 0);
	assert(stackPointer < ((stackPage->realStackLimit)));

	/* begin traceStackOverflow */
	/* begin recordTrace:thing:source: */
	if (TraceLog) {
		traceLog[traceLogIndex] = TraceStackOverflow;
		traceLog[traceLogIndex + 1] = TraceStackOverflow;
		traceLog[traceLogIndex + 2] = 0;
		traceLogIndex = (traceLogIndex + 3) % TraceBufferSize;
	}
	statStackOverflow += 1;

	/* The stack has overflowed this page.  If the system is executing some recursive algorithm,
	   e.g. fibonacci, then the system could thrash overflowing the stack if the call soon returns
	   back to the current page.  To avoid thrashing, since overflow is quite slow, we can move
	   more than one frame.  The idea is to record which page has overflowed, and the first
	   time it overflows move one frame, the second time two frames, and so on.  We move no
	   more frames than would leave the page half occupied. */
	theFP = framePointer;
	if (stackPage == overflowedPage) {
		overflowLimitAddress = ((stackPage->baseAddress)) - overflowLimit;
		overflowCount = (extraFramesToMoveOnOverflow += 1);
		while ((((overflowCount -= 1)) >= 0)
		 && ((((callerFP = ((char *)(longAt(theFP + FoxSavedFP))))) < overflowLimitAddress)
		 && (!((longAt(callerFP + FoxSavedFP)) == 0)))) {
			theFP = callerFP;
		}
	}
	else {
		overflowedPage = stackPage;
		extraFramesToMoveOnOverflow = 0;
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
	newPage = (mostRecentlyUsedPage->nextPage);
	if (!((newPage->baseFP))) {
		goto l1;
	}
	divorceFramesIn(newPage);
	/* end newStackPage */
l1:
	moveFramesInthroughtoPage(stackPage, theFP, newPage);

	/* begin setStackPageAndLimit: */
	assert(newPage);
	stackPage = newPage;
	if (stackLimit != (((char *) (((usqInt) -1))))) {
		stackLimit = (stackPage->stackLimit);
	}
	markStackPageMostRecentlyUsed(newPage);

	/* begin setStackPointersFromPage: */
	stackPointer = (newPage->headSP);
	framePointer = (newPage->headFP);
	assert(!(frameHasContext(framePointer)));
	assert(validInstructionPointerinMethodframePointer(instructionPointer + 1, method, framePointer));
}