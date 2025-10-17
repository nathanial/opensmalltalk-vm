/* Extracted from interp.c:59931 (function printFrameWithSP). */

/*	useful for VM debugging */

	/* StackInterpreter>>#printFrame:WithSP: */

sqInt
printFrameWithSP(char *theFP, char *theSP)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *addr;
    usqInt i;
    usqInt numArgs;
    usqInt numSlots;
    sqInt theMethod;
    char *toDoLimit;
    sqInt topThing;

	if (!(/* couldBeFramePointer: */
			(((((usqInt)theFP)) & (BytesPerWord - 1)) == 0)
		 && ((((((usqInt)theFP)) >= (((usqInt)stackMemory))) && ((((usqInt)theFP)) <= (((usqInt)pages))))))) {
		fprintf(transcript,
				"%p is not in the stack zone?!\n",
				theFP);
		return null;
	}
	theMethod = longAt(theFP + FoxMethod);
	numArgs = byteAt((theFP + FoxFrameFlags) + 1);
	shortPrintFrame(theFP);
	theFP + ((FoxCallerSavedIP + BytesPerWord) + ((numArgs << (shiftForWord()))));

	/* empty frameRange:to: */
	printFrameOopat("rcvr/clsr", theFP + ((FoxCallerSavedIP + BytesPerWord) + ((numArgs << (shiftForWord())))));
	for (i = numArgs; i >= 1; i += -1) {
		printFrameOopat("arg", (theFP + FoxCallerSavedIP) + (i * BytesPerWord));
	}

	/* begin printFrameThing:at: */
	printFrameThingatextraString("cllr ip/ctxt", theFP + FoxCallerSavedIP, ((char *)null));
	printFrameThingatextraString("saved fp", theFP + FoxSavedFP, ((char *)null));
	printFrameOopat("method", theFP + FoxMethod);
	printFrameFlagsForFP(theFP);

	/* begin printFrameThing:at: */
	printFrameThingatextraString("context", theFP + FoxThisContext, ((char *)null));
	printFrameOopat("receiver", theFP + FoxReceiver);
	topThing = longAt(theSP);
	if ((topThing >= theMethod)
	 && (topThing <= (theMethod + (((((/* begin numSlotsOf: */
	assert((classIndexOf(theMethod)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(theMethod + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(theMethod - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) << (shiftForWord()))) + BaseHeaderSize)))) {
		toDoLimit = theSP + BytesPerWord;
		for (addr = ((theFP + FoxReceiver) - BytesPerWord); addr >= toDoLimit; addr += (-BytesPerWord)) {
			printFrameOopat("temp/stck", addr);
		}

		/* begin printFrameThing:at: */
		printFrameThingatextraString("frame ip", theSP, ((char *)null));
	}
	else {
		for (addr = ((theFP + FoxReceiver) - BytesPerWord); addr >= theSP; addr += (-BytesPerWord)) {
			printFrameOopat("temp/stck", addr);
		}
	}
	return 0;
}