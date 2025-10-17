/* Extracted from interp.c:52561 (function externalDivorceFrameandContext). */

/*	Divorce a single frame and its context. If it is not the top frame of a
	stack this means splitting its stack. */
/*	stackPage needs to have current head pointers to avoid confusion. */

	/* StackInterpreter>>#externalDivorceFrame:andContext: */

static NoDbgRegParms void
externalDivorceFrameandContext(char *theFP, sqInt ctxt)
{
    sqInt callerCtx;
    char *callerFP;
    char *callerFP1;
    sqInt callerIP;
    char *callerSP;
    char *frameAbove;
    StackPage *newPage;
    int onCurrent;
    sqInt theIP;
    StackPage *thePage;
    char *theSP;

	assert((!stackPage)
	 || (stackPage == (mostRecentlyUsedPage)));

	/* begin stackPageFor: */
	thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage), pages);
	if (!((onCurrent = thePage == stackPage))) {
		markStackPageNextMostRecentlyUsed(thePage);
	}
	theSP = findSPOfon(theFP, thePage);
	updateStateOfSpouseContextForFrameWithSP(theFP, theSP);

	/* begin ensureCallerContext: */
	callerFP1 = ((char *)(longAt(theFP + FoxSavedFP)));
	if (!callerFP1) {
		/* begin frameCallerContext: */
		assert(isBaseFrame(theFP));
		callerCtx = longAt(theFP + FoxCallerContext);
		goto l2;
	}

	/* base frame, context in saved ip slot (or base of stack in Cog) */

	/* begin ensureFrameIsMarried:SP: */
	if (byteAt((callerFP1 + FoxFrameFlags) + 2)) {
		assert(isContext(frameContext(callerFP1)));
		callerCtx = longAt(callerFP1 + FoxThisContext);
		goto l2;
	}
	callerCtx = marryFrameSP(
			callerFP1,
			(/* begin frameCallerStackPointer: */
				assert(!(isBaseFrame(theFP))),
			(theFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord));
	/* end ensureCallerContext: */
l2:
	if ((frameAbove = findFrameAboveinPage(theFP, thePage))) {
		/* begin newStackPage */
		newPage = (mostRecentlyUsedPage->nextPage);
		if (!((newPage->baseFP))) {
			goto l1;
		}
		divorceFramesIn(newPage);
		/* end newStackPage */
l1:
		theIP = longAt(frameAbove + FoxCallerSavedIP);
		frameAbove = moveFramesInthroughtoPage(thePage, frameAbove, newPage);
		if (onCurrent) {
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
		}
		else {
			markStackPageMostRecentlyUsed(newPage);
		}
		assert((frameCallerContext(frameAbove)) == ctxt);
	}
	else {
		theIP = longAt((thePage->headSP));
	}

	/* If we're divorcing the top frame we can simply peel it off.
	   othewise move all frames above to a new stack and then peel the frame off. */

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(ctxt))
	 && (!(isForwarded(ctxt))));
	assert(validStorePointerUncheckedArgs(
		InstructionPointerIndex,
		ctxt,
		(/* begin contextInstructionPointer:frame: */
			assert(validInstructionPointerinFrame(theIP + 1, theFP)),
		(((usqInt)(((theIP - (longAt(theFP + FoxMethod))) - BaseHeaderSize) + 2) << 3) | 1))));
	longAtput((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),(/* begin contextInstructionPointer:frame: */
		assert(validInstructionPointerinFrame(theIP + 1, theFP)),
	(((usqInt)(((theIP - (longAt(theFP + FoxMethod))) - BaseHeaderSize) + 2) << 3) | 1)));

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(SenderIndex, ctxt, callerCtx));
	assert(isNonImmediate(ctxt));
	if (oopisGreaterThanOrEqualTo(ctxt, oldSpaceStart)) {
		if (/* isYoung: */
			((!(callerCtx & (tagMask()))))
		 && (oopisLessThan(callerCtx, oldSpaceStart))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(ctxt + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(ctxt);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),callerCtx);
	callerFP = ((char *)(longAt(theFP + FoxSavedFP)));
	if (callerFP) {
		callerIP = longAt(theFP + FoxCallerSavedIP);
		callerSP = ((/* begin frameCallerSP: */
	assert(!(isBaseFrame(theFP))),
(theFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord)) - BytesPerWord;
		longAtput(callerSP,callerIP);

		/* begin setHeadFP:andSP:inPage: */
		assert(callerSP < callerFP);
		assert((callerSP < ((thePage->baseAddress)))
		 && (callerSP > (((thePage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
		assert((callerFP < ((thePage->baseAddress)))
		 && (callerFP > (((thePage->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
		(thePage->headFP = callerFP);
		(thePage->headSP = callerSP);
	}
	else {
		freeStackPage(thePage);
	}
}