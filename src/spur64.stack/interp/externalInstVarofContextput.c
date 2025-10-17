/* Extracted from interp.c:52776 (function externalInstVarofContextput). */

	/* StackInterpreter>>#externalInstVar:ofContext:put: */

static NoDbgRegParms sqInt
externalInstVarofContextput(sqInt index, sqInt maybeMarriedContext, sqInt anOop)
{
    int onCurrentPage;
    sqInt senderOop;
    char *theFP;
    StackPage *thePage;

	assert(isContext(maybeMarriedContext));

	/* begin externalWriteBackHeadFramePointers */
	assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
	assert(stackPage == (mostRecentlyUsedPage));
	assert(!((isFree(stackPage))));

	/* begin setHeadFP:andSP:inPage: */
	assert(stackPointer < framePointer);
	assert((stackPointer < ((stackPage->baseAddress)))
	 && (stackPointer > (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
	assert((framePointer < ((stackPage->baseAddress)))
	 && (framePointer > (((stackPage->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
	(stackPage->headFP = framePointer);
	(stackPage->headSP = stackPointer);
	assert(pageListIsWellFormed());

	/* Assign the field of a married context. */
	assert(!((isObjImmutable(maybeMarriedContext))));
	if (!(/* isStillMarriedContext: */
			(((((longAt((void *)((maybeMarriedContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(maybeMarriedContext))))) {
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(index, maybeMarriedContext, anOop));
		assert(isNonImmediate(maybeMarriedContext));
		if (oopisGreaterThanOrEqualTo(maybeMarriedContext, oldSpaceStart)) {
			if (/* isYoung: */
				((!(anOop & (tagMask()))))
			 && (oopisLessThan(anOop, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(maybeMarriedContext + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(maybeMarriedContext);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((maybeMarriedContext + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))),anOop);
		return null;
	}

	/* begin frameOfMarriedContext: */
	senderOop = longAt((void *)((maybeMarriedContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
	assert((((senderOop) & 7) == 1));
	theFP = ((char *)(senderOop - (smallIntegerTag())));

	/* begin stackPageFor: */
	thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage), pages);
	assert(stackPage == (mostRecentlyUsedPage));
	onCurrentPage = thePage == stackPage;
	if (index) {
		externalDivorceFrameandContext(theFP, maybeMarriedContext);

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(index, maybeMarriedContext, anOop));
		assert(isNonImmediate(maybeMarriedContext));
		if (oopisGreaterThanOrEqualTo(maybeMarriedContext, oldSpaceStart)) {
			if (/* isYoung: */
				((!(anOop & (tagMask()))))
			 && (oopisLessThan(anOop, oldSpaceStart))) {
				/* begin possibleRootStoreInto: */
				if (!((byteAt((void *)(maybeMarriedContext + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
					remember(maybeMarriedContext);
				}
			}
		}

		/* most stores into young objects */
		longAtput((void *)((maybeMarriedContext + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))),anOop);
	}
	else {
		storeSenderOfFramewithValue(theFP, anOop);
	}
	if (onCurrentPage) {
		/* begin setStackPointersFromPage: */
		stackPointer = (stackPage->headSP);
		framePointer = (stackPage->headFP);
	}
	else {
		markStackPageMostRecentlyUsed(stackPage);
	}
	assert(stackPage == (mostRecentlyUsedPage));
	assert(pageListIsWellFormed());
	assert(validStackPageBaseFrames());
	return 0;
}