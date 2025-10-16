/* Extracted from interp.c:64622 (function stObjectatput). */

sqInt
stObjectatput(sqInt array, sqInt index, sqInt value)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt class;
    sqInt fieldIndex;
    sqInt fixedFields;
    usqLong fmt;
    sqLong hdr;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt signedValueToStore;
    usqInt sp;
    sqInt spSqInt;
    sqInt stSize;
    sqInt totalLength;
    usqLong unsigned64BitValueToStore;
    unsigned int unsignedValueToStore;

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
		goto l1;
	}
	if (fmt >= (firstByteFormat())) {
		totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
		goto l1;
	}

	/* bytes, including CompiledMethod */
	if (fmt >= (firstShortFormat())) {
		totalLength = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
		goto l1;
	}
	if (fmt >= (firstLongFormat())) {
		totalLength = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
		goto l1;
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		totalLength = numSlots;
		goto l1;
	}

	/* fmt = self forwardedFormat */
	totalLength = 0;
	/* end lengthOf:baseHeader:format: */
l1:

	/* begin fixedFieldsOf:format:length: */
	if ((fmt >= (sixtyFourBitIndexableFormat()))
	 || (fmt == (arrayFormat()))) {
		fixedFields = 0;
		goto l2;
	}
	if (fmt < (arrayFormat())) {
		fixedFields = totalLength;
		goto l2;
	}
	class = fetchClassOfNonImm(array);
	fixedFields = (((longAt((void *)((class + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)) & ((1U << (fixedFieldsFieldWidth())) - 1);
	/* end fixedFieldsOf:format:length: */
l2:
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
			return temporaryinput(index - 1, frameOfMarriedContext(array), value);
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
		/* begin subscript:with:storing:format: */
		if (fmt <= 5 /* lastPointerFormat */) {
			fieldIndex = (index + fixedFields) - 1;

			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(fieldIndex, array, value));
			assert(isNonImmediate(array));
			if (oopisGreaterThanOrEqualTo(array, GIV(oldSpaceStart))) {
				if (/* isYoung: */
					((!(value & (tagMask()))))
				 && (oopisLessThan(value, GIV(oldSpaceStart)))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(array + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(array);
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((array + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),value);
			goto l4;
		}
		if (fmt >= (firstByteFormat())) {
			if (!((((value) & 7) == 1))) {
				GIV(primFailCode) = PrimErrBadArgument;
				goto l4;
			}
			signedValueToStore = (value >> 3);
			if (!((signedValueToStore >= 0)
				 && (signedValueToStore <= 0xFF))) {
				GIV(primFailCode) = PrimErrBadArgument;
				goto l4;
			}

			/* storeByte:ofObject:withValue: */
			byteAtput((void *)((array + BaseHeaderSize) + ((index + fixedFields) - 1)),signedValueToStore);
			goto l4;
		}
		if (fmt >= (firstShortFormat())) {
			if (!((((value) & 7) == 1))) {
				GIV(primFailCode) = PrimErrBadArgument;
				goto l4;
			}
			signedValueToStore = (value >> 3);
			if (!((signedValueToStore >= 0)
				 && (signedValueToStore <= 0xFFFF))) {
				GIV(primFailCode) = PrimErrBadArgument;
				goto l4;
			}

			/* storeShort16:ofObject:withValue: */
			shortAtput((void *)((array + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << 1)))),signedValueToStore);
			goto l4;
		}
		if (fmt == (sixtyFourBitIndexableFormat())) {
			unsigned64BitValueToStore = positive64BitValueOf(value);
			if (!GIV(primFailCode)) {
				/* storeLong64:ofObject:withValue: */
				long64Atput((void *)((array + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << 3)))),unsigned64BitValueToStore);
			}
			goto l4;
		}

		/* 32bit-word type objects */
		unsignedValueToStore = positive32BitValueOf(value);
		if (!GIV(primFailCode)) {
			/* storeLong32:ofObject:withValue: */
			long32Atput((void *)((array + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << 2)))),unsignedValueToStore);
		}
		/* end subscript:with:storing:format: */
l4:;
	}
	else {
		/* primitiveFailFor: */
		GIV(primFailCode) = (fmt <= 1
					? PrimErrBadReceiver
					: PrimErrBadIndex);
	}
	return value;
}