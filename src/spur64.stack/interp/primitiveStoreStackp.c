/* Extracted from interp.c:69988 (function primitiveStoreStackp). */

static void
primitiveStoreStackp(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt ctxt;
    sqInt i;
    sqInt integerPointer;
    sqInt newStackp;
    usqInt numSlots;
    int onCurrentPage;
    sqInt senderOop;
    sqInt sp;
    sqInt stackp;
    char *theFP;
    StackPage *thePage;

	ctxt = longAt(GIV(stackPointer) + (1 * BytesPerWord));

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer));
	if ((((integerPointer) & 7) == 1)) {
		newStackp = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		newStackp = 0;
	}
	if (!((!GIV(primFailCode))
		 && (((newStackp >= 0) && (newStackp <= (((/* begin numSlotsOf: */
	assert((classIndexOf(ctxt)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(ctxt + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(ctxt - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - CtxtTempFrameStart)))))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}

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
	if (/* isStillMarriedContext: */
		(((((longAt((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
	 && (!(isWidowedContext(ctxt)))) {
		/* begin frameOfMarriedContext: */
		senderOop = longAt((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
		assert((((senderOop) & 7) == 1));
		theFP = ((char *)(senderOop - (smallIntegerTag())));

		/* begin stackPageFor: */
		thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
		if (((onCurrentPage = thePage == GIV(stackPage)))
		 && (theFP == GIV(framePointer))) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			return;
		}
		externalDivorceFrameandContext(theFP, ctxt);
		if (onCurrentPage) {
			/* begin setStackPointersFromPage: */
			GIV(stackPointer) = (GIV(stackPage)->headSP);
			GIV(framePointer) = (GIV(stackPage)->headFP);
		}
		else {
			assert(GIV(stackPage) == (stackPageFor(GIV(framePointer))));
			markStackPageMostRecentlyUsed(GIV(stackPage));
		}
	}

	/* begin fetchStackPointerOf: */
	sp = longAt((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
	if (!((((sp) & 7) == 1))) {
		stackp = 0;
		goto l1;
	}
	assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(ctxt)));
	stackp = (sp >> 3);
	/* end fetchStackPointerOf: */
l1:

	/* Nil any newly accessible cells */
	for (i = (stackp + 1); i <= newStackp; i += 1) {
		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(ctxt))
		 && (!(isForwarded(ctxt))));
		assert(validStorePointerUncheckedArgs((i + CtxtTempFrameStart) - 1, ctxt, GIV(nilObj)));
		longAtput((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(((i + CtxtTempFrameStart) - 1)) << (shiftForWord()))))),GIV(nilObj));
	}

	/* begin storeStackPointerValue:inContext: */
	assert((ReceiverIndex + newStackp) < (lengthOf(ctxt)));
	assert((isNonImmediate(ctxt))
	 && (!(isForwarded(ctxt))));
	assert(validStorePointerUncheckedArgs(StackPointerIndex, ctxt, (((usqInt)newStackp << 3) | 1)));
	longAtput((void *)((ctxt + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))),(((usqInt)newStackp << 3) | 1));

	/* begin pop: */
	GIV(stackPointer) += 1 * BytesPerWord;
}