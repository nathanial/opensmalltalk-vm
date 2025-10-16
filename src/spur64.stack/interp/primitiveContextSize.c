/* Extracted from interp.c:67189 (function primitiveContextSize). */

static void
primitiveContextSize(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt class;
    sqInt fixedFields;
    usqLong fmt;
    sqLong hdr;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt rcvr;
    char *sp;
    sqInt spSqInt;
    usqInt spUsqInt;
    sqInt sz;
    sqInt totalLength;

	rcvr = longAt(GIV(stackPointer));
	hdr = long64At((void *)(rcvr));
	fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

	/* begin lengthOf:baseHeader:format: */
	/* begin lengthOf:format: */
	/* begin numSlotsOfAny: */
	numSlotsUsqInt = byteAt((void *)(rcvr + (numSlotsFieldByteOffset())));
	numSlots = (numSlotsUsqInt == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(rcvr - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	if (fmt <= (ephemeronFormat())) {
		totalLength = numSlots;
		goto l2;
	}
	if (fmt >= (firstByteFormat())) {
		totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
		goto l2;
	}

	/* bytes, including CompiledMethod */
	if (fmt >= (firstShortFormat())) {
		totalLength = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
		goto l2;
	}
	if (fmt >= (firstLongFormat())) {
		totalLength = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
		goto l2;
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		totalLength = numSlots;
		goto l2;
	}

	/* fmt = self forwardedFormat */
	totalLength = 0;
	/* end lengthOf:baseHeader:format: */
l2:

	/* begin fixedFieldsOf:format:length: */
	if ((fmt >= (sixtyFourBitIndexableFormat()))
	 || (fmt == (arrayFormat()))) {
		fixedFields = 0;
		goto l1;
	}
	if (fmt < (arrayFormat())) {
		fixedFields = totalLength;
		goto l1;
	}
	class = fetchClassOfNonImm(rcvr);
	fixedFields = (((longAt((void *)((class + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)) & ((1U << (fixedFieldsFieldWidth())) - 1);
	/* end fixedFieldsOf:format:length: */
l1:
	if ((hdr & (classIndexMask())) == ClassMethodContextCompactIndex) {
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

		/* begin stackPointerForMaybeMarriedContext: */
		if (/* isStillMarriedContext: */
			(((((longAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(rcvr)))) {
			spUsqInt = stackPointerIndexForFrame(frameOfMarriedContext(rcvr));
			assert((ReceiverIndex + ((spUsqInt >> 3))) < (lengthOf(rcvr)));
			sz = spUsqInt;
			goto l3;
		}

		/* begin fetchStackPointerOf: */
		spSqInt = longAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
		if (!((((spSqInt) & 7) == 1))) {
			sz = 0;
			goto l3;
		}
		assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(rcvr)));
		sz = (spSqInt >> 3);
		/* end stackPointerForMaybeMarriedContext: */
l3:;
	}
	else {
		sz = totalLength - fixedFields;
	}

	/* begin methodReturnInteger: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)sz << 3) | 1));
	GIV(stackPointer) = sp;
}