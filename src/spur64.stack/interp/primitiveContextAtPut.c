/* Extracted from interp.c:66773 (function primitiveContextAtPut). */

/*	Special version of primitiveAtPut for accessing contexts.
	Written to be varargs for use from mirror primitives. */

	/* StackInterpreterPrimitives>>#primitiveContextAtPut */

static void
primitiveContextAtPut(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aContext;
    sqInt class;
    sqInt fieldIndex;
    sqInt fixedFields;
    sqInt fixedFieldsSqInt;
    usqLong fmt;
    usqLong fmtUsqLong;
    usqInt frameNumArgs;
    sqLong hdr;
    sqLong hdrSqLong;
    sqInt index;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt senderOop;
    sqInt signedValueToStore;
    char *sp;
    char *spouseFP;
    sqInt spSqInt;
    usqInt spUsqInt;
    sqInt stSize;
    sqInt stSizeSqInt;
    sqInt totalLength;
    sqInt totalLengthSqInt;
    usqLong unsigned64BitValueToStore;
    unsigned int unsignedValueToStore;
    sqInt value;

	value = longAt(GIV(stackPointer));
	index = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	aContext = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	if (!((((index) & 7) == 1))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}

	/* Duplicating much of stObject:at:put: here allows stObject:at:put: to omit tests for contexts. */
	hdr = long64At((void *)(aContext));
	index = (index >> 3);
	if (!((hdr & (classIndexMask())) == ClassMethodContextCompactIndex)) {
		/* begin stObject:at:put: */
		hdrSqLong = long64At((void *)(aContext));
		fmtUsqLong = (((usqLong)(hdrSqLong)) >> (formatShift())) & (formatMask());

		/* begin lengthOf:baseHeader:format: */
		/* begin lengthOf:format: */
		/* begin numSlotsOfAny: */
		numSlotsUsqInt = byteAt((void *)(aContext + (numSlotsFieldByteOffset())));
		numSlots = (numSlotsUsqInt == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(aContext - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlotsUsqInt);
		if (fmtUsqLong <= (ephemeronFormat())) {
			totalLengthSqInt = numSlots;
			goto l6;
		}
		if (fmtUsqLong >= (firstByteFormat())) {
			totalLengthSqInt = ((numSlots << (shiftForWord()))) - (fmtUsqLong & 7);
			goto l6;
		}

		/* bytes, including CompiledMethod */
		if (fmtUsqLong >= (firstShortFormat())) {
			totalLengthSqInt = ((numSlots << ((shiftForWord()) - 1))) - (fmtUsqLong & 3);
			goto l6;
		}
		if (fmtUsqLong >= (firstLongFormat())) {
			totalLengthSqInt = ((numSlots << ((shiftForWord()) - 2))) - (fmtUsqLong & 1);
			goto l6;
		}
		if (fmtUsqLong == (sixtyFourBitIndexableFormat())) {
			totalLengthSqInt = numSlots;
			goto l6;
		}

		/* fmt = self forwardedFormat */
		totalLengthSqInt = 0;
		/* end lengthOf:baseHeader:format: */
l6:

		/* begin fixedFieldsOf:format:length: */
		if ((fmtUsqLong >= (sixtyFourBitIndexableFormat()))
		 || (fmtUsqLong == (arrayFormat()))) {
			fixedFieldsSqInt = 0;
			goto l8;
		}
		if (fmtUsqLong < (arrayFormat())) {
			fixedFieldsSqInt = totalLengthSqInt;
			goto l8;
		}
		class = fetchClassOfNonImm(aContext);
		fixedFieldsSqInt = (((longAt((void *)((class + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)) & ((1U << (fixedFieldsFieldWidth())) - 1);
		/* end fixedFieldsOf:format:length: */
l8:
		if ((fmtUsqLong == (indexablePointersFormat()))
		 && ((hdrSqLong & (classIndexMask())) == ClassMethodContextCompactIndex)) {
			/* begin stackPointerForMaybeMarriedContext: */
			if (/* isStillMarriedContext: */
				(((((longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
			 && (!(isWidowedContext(aContext)))) {
				spUsqInt = stackPointerIndexForFrame(frameOfMarriedContext(aContext));
				assert((ReceiverIndex + ((spUsqInt >> 3))) < (lengthOf(aContext)));
				stSizeSqInt = spUsqInt;
				goto l5;
			}

			/* begin fetchStackPointerOf: */
			spSqInt = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
			if (!((((spSqInt) & 7) == 1))) {
				stSizeSqInt = 0;
				goto l5;
			}
			assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(aContext)));
			stSizeSqInt = (spSqInt >> 3);
			/* end stackPointerForMaybeMarriedContext: */
l5:
			if ((oopisGreaterThanOrEqualTo(index, 1))
			 && ((oopisLessThanOrEqualTo(index, stSizeSqInt))
			 && (/* isStillMarriedContext: */
				(((((longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
			 && (!(isWidowedContext(aContext)))))) {
				temporaryinput(index - 1, frameOfMarriedContext(aContext), value);
				goto l9;
			}
		}
		else {
			stSizeSqInt = totalLengthSqInt - fixedFieldsSqInt;
		}
		if ((oopisGreaterThanOrEqualTo(index, /* firstValidIndexOfIndexableObject:withFormat: */
			(fmtUsqLong >= (firstCompiledMethodFormat())
				? (((literalCountOf(aContext)) + LiteralStart) * BytesPerOop) + 1
				: 1)))
		 && (oopisLessThanOrEqualTo(index, stSizeSqInt))) {
			/* begin subscript:with:storing:format: */
			if (fmtUsqLong <= 5 /* lastPointerFormat */) {
				fieldIndex = (index + fixedFieldsSqInt) - 1;

				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(fieldIndex, aContext, value));
				assert(isNonImmediate(aContext));
				if (oopisGreaterThanOrEqualTo(aContext, GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(value & (tagMask()))))
					 && (oopisLessThan(value, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(aContext + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(aContext);
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((aContext + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),value);
				goto l7;
			}
			if (fmtUsqLong >= (firstByteFormat())) {
				if (!((((value) & 7) == 1))) {
					GIV(primFailCode) = PrimErrBadArgument;
					goto l7;
				}
				signedValueToStore = (value >> 3);
				if (!((signedValueToStore >= 0)
					 && (signedValueToStore <= 0xFF))) {
					GIV(primFailCode) = PrimErrBadArgument;
					goto l7;
				}

				/* storeByte:ofObject:withValue: */
				byteAtput((void *)((aContext + BaseHeaderSize) + ((index + fixedFieldsSqInt) - 1)),signedValueToStore);
				goto l7;
			}
			if (fmtUsqLong >= (firstShortFormat())) {
				if (!((((value) & 7) == 1))) {
					GIV(primFailCode) = PrimErrBadArgument;
					goto l7;
				}
				signedValueToStore = (value >> 3);
				if (!((signedValueToStore >= 0)
					 && (signedValueToStore <= 0xFFFF))) {
					GIV(primFailCode) = PrimErrBadArgument;
					goto l7;
				}

				/* storeShort16:ofObject:withValue: */
				shortAtput((void *)((aContext + BaseHeaderSize) + ((((usqInt)(((index + fixedFieldsSqInt) - 1)) << 1)))),signedValueToStore);
				goto l7;
			}
			if (fmtUsqLong == (sixtyFourBitIndexableFormat())) {
				unsigned64BitValueToStore = positive64BitValueOf(value);
				if (!GIV(primFailCode)) {
					/* storeLong64:ofObject:withValue: */
					long64Atput((void *)((aContext + BaseHeaderSize) + ((((usqInt)(((index + fixedFieldsSqInt) - 1)) << 3)))),unsigned64BitValueToStore);
				}
				goto l7;
			}

			/* 32bit-word type objects */
			unsignedValueToStore = positive32BitValueOf(value);
			if (!GIV(primFailCode)) {
				/* storeLong32:ofObject:withValue: */
				long32Atput((void *)((aContext + BaseHeaderSize) + ((((usqInt)(((index + fixedFieldsSqInt) - 1)) << 2)))),unsignedValueToStore);
			}
			/* end subscript:with:storing:format: */
l7:;
		}
		else {
			/* primitiveFailFor: */
			GIV(primFailCode) = (fmtUsqLong <= 1
						? PrimErrBadReceiver
						: PrimErrBadIndex);
		}
		/* end stObject:at:put: */
l9:
		if (!GIV(primFailCode)) {
			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),value);
			GIV(stackPointer) = sp;
		}
		return;
	}

	/* might be an instance of a subclass */

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
	if (!(/* isStillMarriedContext: */
			(((((longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
		 && (!(isWidowedContext(aContext))))) {
		fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

		/* begin lengthOf:baseHeader:format: */
		/* begin lengthOf:format: */
		/* begin numSlotsOfAny: */
		numSlotsUsqInt = byteAt((void *)(aContext + (numSlotsFieldByteOffset())));
		numSlots = (numSlotsUsqInt == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(aContext - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlotsUsqInt);
		if (fmt <= (ephemeronFormat())) {
			totalLength = numSlots;
			goto l3;
		}
		if (fmt >= (firstByteFormat())) {
			totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
			goto l3;
		}

		/* bytes, including CompiledMethod */
		if (fmt >= (firstShortFormat())) {
			totalLength = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
			goto l3;
		}
		if (fmt >= (firstLongFormat())) {
			totalLength = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
			goto l3;
		}
		if (fmt == (sixtyFourBitIndexableFormat())) {
			totalLength = numSlots;
			goto l3;
		}

		/* fmt = self forwardedFormat */
		totalLength = 0;
		/* end lengthOf:baseHeader:format: */
l3:

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
		class = fetchClassOfNonImm(aContext);
		fixedFields = (((longAt((void *)((class + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)) & ((1U << (fixedFieldsFieldWidth())) - 1);
		/* end fixedFieldsOf:format:length: */
l2:

		/* begin fetchStackPointerOf: */
		spSqInt = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
		if (!((((spSqInt) & 7) == 1))) {
			stSize = 0;
			goto l1;
		}
		assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(aContext)));
		stSize = (spSqInt >> 3);
		/* end fetchStackPointerOf: */
l1:
		if (!(((index >= 1) && (index <= stSize)))) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadIndex;
			return;
		}

		/* begin subscript:with:storing:format: */
		if (fmt <= 5 /* lastPointerFormat */) {
			fieldIndex = (index + fixedFields) - 1;

			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(fieldIndex, aContext, value));
			assert(isNonImmediate(aContext));
			if (oopisGreaterThanOrEqualTo(aContext, GIV(oldSpaceStart))) {
				if (/* isYoung: */
					((!(value & (tagMask()))))
				 && (oopisLessThan(value, GIV(oldSpaceStart)))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(aContext + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(aContext);
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((aContext + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),value);
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
			byteAtput((void *)((aContext + BaseHeaderSize) + ((index + fixedFields) - 1)),signedValueToStore);
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
			shortAtput((void *)((aContext + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << 1)))),signedValueToStore);
			goto l4;
		}
		if (fmt == (sixtyFourBitIndexableFormat())) {
			unsigned64BitValueToStore = positive64BitValueOf(value);
			if (!GIV(primFailCode)) {
				/* storeLong64:ofObject:withValue: */
				long64Atput((void *)((aContext + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << 3)))),unsigned64BitValueToStore);
			}
			goto l4;
		}

		/* 32bit-word type objects */
		unsignedValueToStore = positive32BitValueOf(value);
		if (!GIV(primFailCode)) {
			/* storeLong32:ofObject:withValue: */
			long32Atput((void *)((aContext + BaseHeaderSize) + ((((usqInt)(((index + fixedFields) - 1)) << 2)))),unsignedValueToStore);
		}
		/* end subscript:with:storing:format: */
l4:

		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),value);
		GIV(stackPointer) = sp;
		return;
	}

	/* begin frameOfMarriedContext: */
	senderOop = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
	assert((((senderOop) & 7) == 1));
	spouseFP = ((char *)(senderOop - (smallIntegerTag())));
	if (!(((index >= 1) && (index <= (stackPointerIndexForFrame(spouseFP)))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}

	/* temporary:in:put: */
	if ((index - 1) < ((frameNumArgs = byteAt((spouseFP + FoxFrameFlags) + 1)))) {
		longAtput((spouseFP + FoxCallerSavedIP) + ((frameNumArgs - (index - 1)) * BytesPerWord),value);
	}
	else {
		longAtput(((spouseFP + FoxReceiver) - BytesPerWord) + ((frameNumArgs - (index - 1)) * BytesPerWord),value);
	}

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),value);
	GIV(stackPointer) = sp;
}