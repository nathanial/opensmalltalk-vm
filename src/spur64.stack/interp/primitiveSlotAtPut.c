/* Extracted from interp.c:69782 (function primitiveSlotAtPut). */

static void
primitiveSlotAtPut(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt fmt;
    sqInt fmtSqInt;
    sqInt index;
    sqInt newValue;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt rcvr;
    char *sp;
    usqIntptr_t value;
    sqInt valueSqInt;

	newValue = longAt(GIV(stackPointer));
	index = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	rcvr = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	if (!((((index) & 7) == 1))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	if (((rcvr & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		return;
	}
	if (
#  if IMMUTABILITY
		((((usqInt)((byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0
#  else
		0
#  endif
		) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrNoModification;
		return;
	}
	fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
	index = ((index >> 3)) - 1;
	if (fmt <= 5 /* lastPointerFormat */) {
		/* begin numSlotsOf: */
		assert((classIndexOf(rcvr)) > (isForwardedObjectClassIndexPun()));
		numSlots = (((numSlotsUsqInt = byteAt((void *)(rcvr + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(rcvr - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlotsUsqInt);
		if ((((usqInt)index)) < numSlots) {
			if (((longAt((void *)(rcvr))) & (classIndexMask())) == ClassMethodContextCompactIndex) {
				externalInstVarofContextput(index, rcvr, newValue);
			}
			else {
				/* begin storePointer:ofObject:withValue: */
				assert(validStorePointerArgs(index, rcvr, newValue));
				assert(isNonImmediate(rcvr));
				if (oopisGreaterThanOrEqualTo(rcvr, GIV(oldSpaceStart))) {
					if (/* isYoung: */
						((!(newValue & (tagMask()))))
					 && (oopisLessThan(newValue, GIV(oldSpaceStart)))) {
						/* begin possibleRootStoreInto: */
						if (!((byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
							remember(rcvr);
						}
					}
				}

				/* most stores into young objects */
				longAtput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))),newValue);
			}

			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),newValue);
			GIV(stackPointer) = sp;
			return;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}

	/* begin positiveMachineIntegerValueOf: */
	if ((((newValue) & 7) == 1)) {
		valueSqInt = (newValue >> 3);
		if (valueSqInt < 0) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			value = null;
			goto l1;
		}
		value = valueSqInt;
		goto l1;
	}

	/* don't inline the rare case */
	value = positiveMachineIntegerValueOfObj(newValue);
	/* end positiveMachineIntegerValueOf: */
l1:
	if (GIV(primFailCode)) {
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	if (fmt >= (firstByteFormat())) {
		if (fmt >= (firstCompiledMethodFormat())) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrUnsupported;
			return;
		}
		if ((((usqInt)value)) > 0xFF) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
			return;
		}

		/* begin numBytesOfBytes: */
		fmtSqInt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
		assert(fmtSqInt >= (firstByteFormat()));
		numSlots = ((((/* begin numSlotsOf: */
	assert((classIndexOf(rcvr)) > (isForwardedObjectClassIndexPun())),
(((numSlotsUsqInt = byteAt((void *)(rcvr + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(rcvr - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlotsUsqInt))) << (shiftForWord()))) - (fmtSqInt & 7);
		if ((((usqInt)index)) < numSlots) {
			/* storeByte:ofObject:withValue: */
			byteAtput((void *)((rcvr + BaseHeaderSize) + index),value);

			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),newValue);
			GIV(stackPointer) = sp;
			return;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}
	if (fmt >= (firstShortFormat())) {
		if ((((usqInt)value)) > 0xFFFF) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
			return;
		}
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 1;
		if ((((usqInt)index)) < numSlots) {
			/* storeShort16:ofObject:withValue: */
			shortAtput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 1)))),value);

			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),newValue);
			GIV(stackPointer) = sp;
			return;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 3;
		if ((((usqInt)index)) < numSlots) {
			/* storeLong64:ofObject:withValue: */
			long64Atput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 3)))),value);

			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),newValue);
			GIV(stackPointer) = sp;
			return;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}
	if (fmt >= (firstLongFormat())) {
		if ((((usqInt)value)) > 0xFFFFFFFFU) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
			return;
		}
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 2;
		if ((((usqInt)index)) < numSlots) {
			/* storeLong32:ofObject:withValue: */
			long32Atput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 2)))),value);

			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),newValue);
			GIV(stackPointer) = sp;
			return;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadReceiver;
	return;
}