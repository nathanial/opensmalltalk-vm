/* Extracted from interp.c:57957 (function moveFramesInthroughtoPage). */

static NoDbgRegParms char *
moveFramesInthroughtoPage(StackPage *oldPage, char *theFP, StackPage *newPage)
{
    char *callerFP;
    sqInt callerIP;
    sqInt delta;
    char *fpInNewPage;
    char *newFP;
    char *newSP;
    char *offsetCallerFP;
    char *source;
    sqInt stackedReceiverOffset;
    sqInt theContext;
    char *theFP1;
    sqInt valuePointer;
    sqInt valuePointerSqInt;

	newSP = ((newPage->baseAddress)) + BytesPerWord;
	stackedReceiverOffset = (FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord()))));

	/* First move the data.  We will fix up frame pointers later. */
	for (source = (theFP + stackedReceiverOffset); source >= ((oldPage->headSP)); source += (-BytesPerWord)) {
		newSP -= BytesPerWord;
		longAtput(newSP,longAt(source));
	}

	/* newSP = oldSP + delta => delta = newSP - oldSP */
	delta = newSP - ((oldPage->headSP));
	newFP = ((newPage->baseAddress)) - stackedReceiverOffset;
	theFP1 = ((oldPage->headFP)) + delta;

	/* begin setHeadFP:andSP:inPage: */
	assert(newSP < theFP1);
	assert((newSP < ((newPage->baseAddress)))
	 && (newSP > (((newPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
	assert((theFP1 < ((newPage->baseAddress)))
	 && (theFP1 > (((newPage->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
	(newPage->headFP = theFP1);
	(newPage->headSP = newSP);
	(newPage->baseFP = newFP);
	callerFP = ((char *)(longAt(theFP + FoxSavedFP)));
	assert(!(isBaseFrame(theFP)));
	assert(frameHasContext(callerFP));
	callerIP = longAt(theFP + FoxCallerSavedIP);
	longAtput(theFP + stackedReceiverOffset,callerIP);
	assert((callerFP < ((oldPage->baseAddress)))
	 && (callerFP > (((oldPage->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
	(oldPage->headFP = callerFP);
	(oldPage->headSP = theFP + stackedReceiverOffset);

	/* Mark the new base frame in the new page (FoxCallerContext a.k.a. FoxCallerSavedIP) */
	longAtput(newFP + FoxCallerContext,longAt(callerFP + FoxThisContext));
	longAtput(newFP + FoxSavedFP,0);

	/* Now relocate frame pointers, updating married contexts to refer to their moved spouse frames. */
	fpInNewPage = (newPage->headFP);
	do {
		offsetCallerFP = ((char *)(longAt(fpInNewPage + FoxSavedFP)));
		if (offsetCallerFP) {
			offsetCallerFP += delta;
		}
		longAtput(fpInNewPage + FoxSavedFP,oopForPointer(offsetCallerFP));
		if (byteAt((fpInNewPage + FoxFrameFlags) + 2)) {
			theContext = longAt(fpInNewPage + FoxThisContext);
			/* begin withSmallIntegerTags: */
			assert(((oopForPointer(fpInNewPage)) & (BytesPerWord - 1)) == 0);
			valuePointer = (oopForPointer(fpInNewPage)) + (smallIntegerTag());

			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(theContext))
			 && (!(isForwarded(theContext))));
			assert(validStorePointerUncheckedArgs(SenderIndex, theContext, valuePointer));
			longAtput((void *)((theContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),valuePointer);
			/* begin withSmallIntegerTags: */
			assert(((oopForPointer(offsetCallerFP)) & (BytesPerWord - 1)) == 0);
			valuePointerSqInt = (oopForPointer(offsetCallerFP)) + (smallIntegerTag());

			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(theContext))
			 && (!(isForwarded(theContext))));
			assert(validStorePointerUncheckedArgs(InstructionPointerIndex, theContext, valuePointerSqInt));
			longAtput((void *)((theContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),valuePointerSqInt);
		}
		fpInNewPage = offsetCallerFP;
	} while(fpInNewPage != 0);
	return newFP;
}