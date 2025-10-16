/* Extracted from interp.c:52776 (function externalInstVarofContextput). */

static NoDbgRegParms sqInt
externalInstVarofContextput(sqInt index, sqInt maybeMarriedContext, sqInt anOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    int onCurrentPage;
    sqInt senderOop;
    char *theFP;
    StackPage *thePage;

	assert(isContext(maybeMarriedContext));

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

	/* Assign the field of a married context. */
	assert(!((isObjImmutable(maybeMarriedContext))));
	if (!(/* isStillMarriedContext: */
			(((((longAt((void *)((maybeMarriedContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(maybeMarriedContext))))) {
		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(index, maybeMarriedContext, anOop));
		assert(isNonImmediate(maybeMarriedContext));
		if (oopisGreaterThanOrEqualTo(maybeMarriedContext, GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(anOop & (tagMask()))))
			 && (oopisLessThan(anOop, GIV(oldSpaceStart)))) {
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
	thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
	assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
	onCurrentPage = thePage == GIV(stackPage);
	if (index) {
		externalDivorceFrameandContext(theFP, maybeMarriedContext);

		/* begin storePointer:ofObject:withValue: */
		assert(validStorePointerArgs(index, maybeMarriedContext, anOop));
		assert(isNonImmediate(maybeMarriedContext));
		if (oopisGreaterThanOrEqualTo(maybeMarriedContext, GIV(oldSpaceStart))) {
			if (/* isYoung: */
				((!(anOop & (tagMask()))))
			 && (oopisLessThan(anOop, GIV(oldSpaceStart)))) {
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
		GIV(stackPointer) = (GIV(stackPage)->headSP);
		GIV(framePointer) = (GIV(stackPage)->headFP);
	}
	else {
		markStackPageMostRecentlyUsed(GIV(stackPage));
	}
	assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
	assert(pageListIsWellFormed());
	assert(validStackPageBaseFrames());
	return 0;
}