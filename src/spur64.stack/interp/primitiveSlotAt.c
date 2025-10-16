/* Extracted from interp.c:69629 (function primitiveSlotAt). */

static void
primitiveSlotAt(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt fmt;
    sqInt fmtSqInt;
    sqInt index;
    sqInt numLiveSlots;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt oop;
    sqInt rcvr;
    char *sp;
    sqInt value;

	index = longAt(GIV(stackPointer));
	rcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
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
				numLiveSlots = (stackPointerForMaybeMarriedContext(rcvr)) + CtxtTempFrameStart;
				value = ((((usqInt)index)) < numLiveSlots
							? externalInstVarofContext(index, rcvr)
							: GIV(nilObj));
			}
			else {
				value = longAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
			}

			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),value);
			GIV(stackPointer) = sp;
			return;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}
	if (fmt >= (firstByteFormat())) {
		if (fmt >= (firstCompiledMethodFormat())) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrUnsupported;
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
			/* begin pop:thenPushInteger: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)(byteAt((void *)((rcvr + BaseHeaderSize) + index))) << 3) | 1));
			GIV(stackPointer) = sp;
			return;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}
	if (fmt >= (firstShortFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 1;
		if ((((usqInt)index)) < numSlots) {
			/* begin pop:thenPushInteger: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)(((unsigned short) (shortAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 1)))))))) << 3) | 1));
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
			oop = positive64BitIntegerFor(long64At((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 3))))));

			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
			GIV(stackPointer) = sp;
			return;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}
	if (fmt >= (firstLongFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 2;
		if ((((usqInt)index)) < numSlots) {
			/* begin pop:thenPush: */
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),((((((usqInt)(long32At((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 2)))))))) & 0xFFFFFFFFU) << 3) | 1));
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