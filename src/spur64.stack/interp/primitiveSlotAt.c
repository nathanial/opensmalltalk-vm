/* Extracted from interp.c:69629 (function primitiveSlotAt). */

/*	Answer a slot in an object. This numbers all slots from 1, ignoring the
	distinction between
	named and indexed inst vars. In objects with both named and indexed inst
	vars, the named
	inst vars precede the indexed ones. In non-object indexed objects (objects
	that contain
	bits, not object references) this primitive answers the raw integral value
	at each slot. 
	e.g. for Strings it answers the character code, not the Character object
	at each slot. */
/*	because of externalInstVar:ofContext: below */

	/* StackInterpreterPrimitives>>#primitiveSlotAt */

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

	index = longAt(stackPointer);
	rcvr = longAt(stackPointer + (1 * BytesPerWord));
	if (!((((index) & 7) == 1))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	if (((rcvr & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadReceiver;
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
				numLiveSlots = (stackPointerForMaybeMarriedContext(rcvr)) + CtxtTempFrameStart;
				value = ((((usqInt)index)) < numLiveSlots
							? externalInstVarofContext(index, rcvr)
							: nilObj);
			}
			else {
				value = longAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
			}

			/* begin pop:thenPush: */
			longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),value);
			stackPointer = sp;
			return;
		}

		/* primitiveFailFor: */
		primFailCode = PrimErrBadIndex;
		return;
	}
	if (fmt >= (firstByteFormat())) {
		if (fmt >= (firstCompiledMethodFormat())) {
			/* primitiveFailFor: */
			primFailCode = PrimErrUnsupported;
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
			longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),(((usqInt)(byteAt((void *)((rcvr + BaseHeaderSize) + index))) << 3) | 1));
			stackPointer = sp;
			return;
		}

		/* primitiveFailFor: */
		primFailCode = PrimErrBadIndex;
		return;
	}
	if (fmt >= (firstShortFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 1;
		if ((((usqInt)index)) < numSlots) {
			/* begin pop:thenPushInteger: */
			longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),(((usqInt)(((unsigned short) (shortAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 1)))))))) << 3) | 1));
			stackPointer = sp;
			return;
		}

		/* primitiveFailFor: */
		primFailCode = PrimErrBadIndex;
		return;
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 3;
		if ((((usqInt)index)) < numSlots) {
			oop = positive64BitIntegerFor(long64At((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 3))))));

			/* begin pop:thenPush: */
			longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),oop);
			stackPointer = sp;
			return;
		}

		/* primitiveFailFor: */
		primFailCode = PrimErrBadIndex;
		return;
	}
	if (fmt >= (firstLongFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 2;
		if ((((usqInt)index)) < numSlots) {
			/* begin pop:thenPush: */
			longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),((((((usqInt)(long32At((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 2)))))))) & 0xFFFFFFFFU) << 3) | 1));
			stackPointer = sp;
			return;
		}

		/* primitiveFailFor: */
		primFailCode = PrimErrBadIndex;
		return;
	}

	/* primitiveFailFor: */
	primFailCode = PrimErrBadReceiver;
	return;
}