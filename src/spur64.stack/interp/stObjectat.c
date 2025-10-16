/* Extracted from interp.c:64489 (function stObjectat). */

sqInt
stObjectat(sqInt array, sqInt index)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt class;
    sqInt fixedFields;
    usqLong fmt;
    sqLong hdr;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    usqInt sp;
    sqInt spSqInt;
    sqInt stSize;
    sqInt totalLength;

	hdr = long64At((void *)(array));
	fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

	/* begin lengthOf:baseHeader:format: */
	/* begin lengthOf:format: */
	/* begin numSlotsOfAny: */
	numSlotsUsqInt = byteAt((void *)(array + (numSlotsFieldByteOffset())));
	numSlots = (numSlotsUsqInt == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(array - BaseHeaderSize)))) << 8)))))) >> 8
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
	class = fetchClassOfNonImm(array);
	fixedFields = (((longAt((void *)((class + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)) & ((1U << (fixedFieldsFieldWidth())) - 1);
	/* end fixedFieldsOf:format:length: */
l1:
	if ((fmt == (indexablePointersFormat()))
	 && ((hdr & (classIndexMask())) == ClassMethodContextCompactIndex)) {
		/* begin stackPointerForMaybeMarriedContext: */
		if (/* isStillMarriedContext: */
			(((((longAt((void *)((array + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(array)))) {
			sp = stackPointerIndexForFrame(frameOfMarriedContext(array));
			assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(array)));
			stSize = sp;
			goto l3;
		}

		/* begin fetchStackPointerOf: */
		spSqInt = longAt((void *)((array + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
		if (!((((spSqInt) & 7) == 1))) {
			stSize = 0;
			goto l3;
		}
		assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(array)));
		stSize = (spSqInt >> 3);
		/* end stackPointerForMaybeMarriedContext: */
l3:
		if ((oopisGreaterThanOrEqualTo(index, 1))
		 && ((oopisLessThanOrEqualTo(index, stSize))
		 && (/* isStillMarriedContext: */
			(((((longAt((void *)((array + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(array)))))) {
			return temporaryin(index - 1, frameOfMarriedContext(array));
		}
	}
	else {
		stSize = totalLength - fixedFields;
	}
	if ((oopisGreaterThanOrEqualTo(index, /* firstValidIndexOfIndexableObject:withFormat: */
		(fmt >= (firstCompiledMethodFormat())
			? (((literalCountOf(array)) + LiteralStart) * BytesPerOop) + 1
			: 1)))
	 && (oopisLessThanOrEqualTo(index, stSize))) {
		/* begin subscript:with:format: */
		if (fmt <= 5 /* lastPointerFormat */) {
			return longAt((void *)((array + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << (shiftForWord()))))));
		}
		if (fmt >= (firstByteFormat())) {
			return (((usqInt)(byteAt((void *)((array + BaseHeaderSize) + ((index + fixedFields) - 1)))) << 3) | 1);
		}
		if (fmt >= (firstShortFormat())) {
			return (((usqInt)(((unsigned short) (shortAt((void *)((array + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << 1)))))))) << 3) | 1);
		}
		if (fmt == (sixtyFourBitIndexableFormat())) {
			return positive64BitIntegerFor(long64At((void *)((array + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << 3))))));
		}

		/* 32bit-word type objects; for now assume no 64-bit indexable objects */
		return ((((((usqInt)(long32At((void *)((array + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << 2)))))))) & 0xFFFFFFFFU) << 3) | 1);
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = (fmt <= 1
				? PrimErrBadReceiver
				: PrimErrBadIndex);
	return 0;
}