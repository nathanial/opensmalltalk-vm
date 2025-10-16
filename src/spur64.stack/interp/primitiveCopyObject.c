/* Extracted from interp.c:14536 (function primitiveCopyObject). */

static void
primitiveCopyObject(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    sqInt fmt;
    sqInt i;
    usqInt length;
    usqInt numBytes;
    usqInt numSlots;
    sqInt rcvr;
    sqInt valuePointer;

	if (!(GIV(argumentCount) >= 1)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadNumArgs;
		return;
	}
	arg = longAt(GIV(stackPointer));
	rcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (((rcvr & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		return;
	}
	if (((arg & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	if (((longAt((void *)(rcvr))) & (classIndexMask())) != ((longAt((void *)(arg))) & (classIndexMask()))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	if (isWordsOrBytesNonImm(rcvr)) {
		/* begin numBytesOf: */
		fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
		assert((classIndexOf(rcvr)) > (isForwardedObjectClassIndexPun()));
		numBytes = (((numSlots = byteAt((void *)(rcvr + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(rcvr - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots);
		numBytes = (numBytes << (shiftForWord()));
		if (fmt >= (firstByteFormat())) {
			length = numBytes - (fmt & 7);
			goto l1;
		}

		/* bytes (the common case), including CompiledMethod */
		if (fmt <= (sixtyFourBitIndexableFormat())) {
			length = ((sqInt) numBytes);
			goto l1;
		}
		if (fmt >= (firstShortFormat())) {
			length = numBytes - (((fmt & 3) << 1));
			goto l1;
		}

		/* fmt >= self firstLongFormat */
		length = numBytes - (((fmt & 1) << 2));
		/* end numBytesOf: */
l1:
		if (!((((byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask())) == ((byteAt((void *)(arg + (formatFieldByteOffset())))) & (formatMask())))
			 && (length == (numBytesOf(arg))))) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
			return;
		}
		memcpy(((void *)(rcvr + BaseHeaderSize)), ((void *)(arg + BaseHeaderSize)), length);
	}
	else {
		if (!(isAppropriateForCopyObject(rcvr))) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadReceiver;
			return;
		}

		/* begin numSlotsOf: */
		assert((classIndexOf(rcvr)) > (isForwardedObjectClassIndexPun()));
		length = (((numSlots = byteAt((void *)(rcvr + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(rcvr - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots);
		if (!((isAppropriateForCopyObject(arg))
			 && (length == (lengthOf(arg))))) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadArgument;
			return;
		}
		for (i = 0; i < length; i += 1) {
			valuePointer = longAt((void *)((arg + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));

			/* begin storePointer:ofObject:withValue: */
			assert(validStorePointerArgs(i, rcvr, valuePointer));
			assert(isNonImmediate(rcvr));
			if (oopisGreaterThanOrEqualTo(rcvr, GIV(oldSpaceStart))) {
				if (/* isYoung: */
					((!(valuePointer & (tagMask()))))
				 && (oopisLessThan(valuePointer, GIV(oldSpaceStart)))) {
					/* begin possibleRootStoreInto: */
					if (!((byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
						remember(rcvr);
					}
				}
			}

			/* most stores into young objects */
			longAtput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),valuePointer);
		}
	}

	/* Note: The above could be faster for young receivers but I don't think it'll matter */

	/* begin pop: */
	GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
}