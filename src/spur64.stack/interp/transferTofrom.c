/* Extracted from interp.c:65167 (function transferTofrom). */

/*	Record a process to be awoken on the next interpreter cycle.
	Note: It is currently a fatal VM error if there is no runnable process
	(indicated by newProcOrNil being nil), but the right thing to do is to
	enter a wait state until the next event/expired delay/available input,
	etc.  */

	/* StackInterpreter>>#transferTo:from: */

static NoDbgRegParms sqInt
transferTofrom(sqInt newProcOrNil, sqInt sourceCode)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt activeContext;
    StackPage *lastUsedPage;
    sqInt methodHeader;
    sqInt newContext;
    StackPage *newPage;
    sqInt oldProc;
    char *ptr;
    sqInt sched;
    sqInt senderOop;
    char *sp;
    char *theFrame;
    StackPage *thePage;
    char *toDoLimit;
    sqInt top;

	GIV(statProcessSwitch) += 1;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(instructionPointer));
	GIV(stackPointer) = sp;

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

	/* begin assertValidExternalFrameWithIP: */
	assertValidExecutionPointersimbarline(GIV(instructionPointer) + 1, GIV(framePointer), GIV(stackPointer), 1 /* (isMachineCodeFrame: not) */, __LINE__);
	for (ptr = (GIV(stackPointer) + BytesPerWord); ptr <= (GIV(framePointer) + FoxReceiver); ptr += BytesPerWord) {
		assert(addressCouldBeOop(longAt(ptr)));
	}

	/* skip pushed instructionPointer */
	assert(isOopCompiledMethod(frameMethodObject(GIV(framePointer))));
	if (byteAt((GIV(framePointer) + FoxFrameFlags) + 2)) {
		assert(!((isForwarded(frameContext(GIV(framePointer))))));
	}
	toDoLimit = GIV(framePointer) + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((GIV(framePointer) + FoxFrameFlags) + 1))) << (shiftForWord())))));
	for (ptr = ((GIV(framePointer) + FoxCallerSavedIP) + BytesPerWord); ptr <= toDoLimit; ptr += BytesPerWord) {
		assert(addressCouldBeOop(longAt(ptr)));
	}
	sched = longAt((void *)(((longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SchedulerAssociation) << (shiftForWord()))))))) + BaseHeaderSize) + ((((usqInt)(ValueIndex) << (shiftForWord()))))));
	oldProc = longAt((void *)((sched + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));

	/* begin recordContextSwitchFrom:in: */
	/* begin recordTrace:thing:source: */
	if (TraceLog) {
		GIV(traceLog)[GIV(traceLogIndex)] = TraceContextSwitch;
		GIV(traceLog)[GIV(traceLogIndex) + 1] = oldProc;
		GIV(traceLog)[GIV(traceLogIndex) + 2] = sourceCode;
		GIV(traceLogIndex) = (GIV(traceLogIndex) + 3) % TraceBufferSize;
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

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(SuspendedContextIndex, oldProc, activeContext));
	assert(isNonImmediate(oldProc));
	if (oopisGreaterThanOrEqualTo(oldProc, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(activeContext & (tagMask()))))
		 && (oopisLessThan(activeContext, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(oldProc + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(oldProc);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((oldProc + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))),activeContext);
	if (!newProcOrNil) {
		error("scheduler could not find a runnable process");
	}

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(ActiveProcessIndex, sched, newProcOrNil));
	assert(isNonImmediate(sched));
	if (oopisGreaterThanOrEqualTo(sched, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(newProcOrNil & (tagMask()))))
		 && (oopisLessThan(newProcOrNil, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(sched + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(sched);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((sched + BaseHeaderSize) + ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))),newProcOrNil);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(newProcOrNil))
	 && (!(isForwarded(newProcOrNil))));
	assert(validStorePointerUncheckedArgs(MyListIndex, newProcOrNil, GIV(nilObj)));
	longAtput((void *)((newProcOrNil + BaseHeaderSize) + ((((usqInt)(MyListIndex) << (shiftForWord()))))),GIV(nilObj));

	/* begin externalSetStackPageAndPointersForSuspendedContextOfProcess: */
	newContext = longAt((void *)((newProcOrNil + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))));
	assert(isContext(newContext));
	if (((((longAt((void *)((newContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1)) {
		assert(checkIsStillMarriedContextcurrentFP(newContext, GIV(framePointer)));
	}

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(newProcOrNil))
	 && (!(isForwarded(newProcOrNil))));
	assert(validStorePointerUncheckedArgs(SuspendedContextIndex, newProcOrNil, GIV(nilObj)));
	longAtput((void *)((newProcOrNil + BaseHeaderSize) + ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))),GIV(nilObj));
	if (/* isStillMarriedContext: */
		(((((longAt((void *)((newContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
	 && (!(isWidowedContext(newContext)))) {
		/* begin frameOfMarriedContext: */
		senderOop = longAt((void *)((newContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
		assert((((senderOop) & 7) == 1));
		theFrame = ((char *)(senderOop - (smallIntegerTag())));

		/* begin stackPageFor: */
		thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFrame, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
		if (theFrame != ((thePage->headFP))) {
			/* begin newStackPage */
			newPage = (GIV(mostRecentlyUsedPage)->nextPage);
			if (!((newPage->baseFP))) {
				goto l3;
			}
			divorceFramesIn(newPage);
			/* end newStackPage */
l3:
			moveFramesInthroughtoPage(thePage, findFrameAboveinPage(theFrame, thePage), newPage);

			/* begin markStackPageLeastMostRecentlyUsed: */
			assert(newPage == ((GIV(mostRecentlyUsedPage)->nextPage)));
			lastUsedPage = (newPage->nextPage);
			while (((lastUsedPage->baseFP)) == 0) {
				lastUsedPage = (lastUsedPage->nextPage);
			}
			if (((lastUsedPage->nextPage)) == newPage) {
				goto l2;
			}
			(((newPage->prevPage))->nextPage = (newPage->nextPage));
			(((newPage->nextPage))->prevPage = (newPage->prevPage));
			(((lastUsedPage->prevPage))->nextPage = newPage);
			(newPage->prevPage = (lastUsedPage->prevPage));
			(newPage->nextPage = lastUsedPage);
			(lastUsedPage->prevPage = newPage);
			assert(pageListIsWellFormed());
			/* end markStackPageLeastMostRecentlyUsed: */
l2:;
		}

		/* explicit assignment of suspendedContext can cause switch to interior frame. */
		assert(((thePage->headFP)) == theFrame);
	}
	else {
		thePage = makeBaseFrameFor(newContext);
		theFrame = (thePage->baseFP);
	}

	/* begin setStackPageAndLimit: */
	assert(thePage);
	GIV(stackPage) = thePage;
	if (GIV(stackLimit) != (((char *) (((usqInt) -1))))) {
		GIV(stackLimit) = (GIV(stackPage)->stackLimit);
	}
	markStackPageMostRecentlyUsed(thePage);

	/* begin setStackPointersFromPage: */
	GIV(stackPointer) = (thePage->headSP);
	GIV(framePointer) = (thePage->headFP);

	/* begin setMethod: */
	GIV(method) = longAt(GIV(framePointer) + FoxMethod);
	assert(isOopCompiledMethod(GIV(method)));

	/* begin methodUsesAlternateBytecodeSet: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(GIV(method)));
	methodHeader = longAt((void *)((GIV(method) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	if ((((sqLong) methodHeader)) < 0) {
		GIV(bytecodeSetSelector) = 0x100;
	}
	else {
		GIV(bytecodeSetSelector) = 0;
	}

	/* begin popStack */
	top = longAt(GIV(stackPointer));
	GIV(stackPointer) += BytesPerWord;
	GIV(instructionPointer) = top;

	/* begin assertValidExecutionPointe:r:s: */
	assertValidExecutionPointersimbarline(GIV(instructionPointer), GIV(framePointer), GIV(stackPointer), 1 /* (isMachineCodeFrame: not) */, __LINE__);
	return 0;
}