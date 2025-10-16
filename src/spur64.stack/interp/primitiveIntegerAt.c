/* Extracted from interp.c:18007 (function primitiveIntegerAt). */

static void
primitiveIntegerAt(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classFormat;
    sqInt classPointer;
    sqInt fmt;
    sqInt fmtSqInt;
    sqInt index;
    sqInt numFixed;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt oop;
    sqInt rcvr;
    char *sp;
    sqInt value;

	/* begin primitiveSpurIntegerAt */
	index = longAt(GIV(stackPointer));
	rcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (!((((index) & 7) == 1))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		goto l1;
	}
	if (((rcvr & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		goto l1;
	}
	fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
	index = ((index >> 3)) - 1;
	if (fmt >= (firstByteFormat())) {
		if (fmt >= (firstCompiledMethodFormat())) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrUnsupported;
			goto l1;
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
			value = byteAt((void *)((rcvr + BaseHeaderSize) + index));
			if (value > 0x7F) {
				value -= 0x100;
			}

			/* begin methodReturnInteger: */
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)value << 3) | 1));
			GIV(stackPointer) = sp;
			goto l1;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		goto l1;
	}
	if (fmt >= (firstShortFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 1;
		if ((((usqInt)index)) < numSlots) {
			value = shortAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 1)))));
			if (value > 0x7FFF) {
				value -= 0x10000;
			}

			/* begin methodReturnInteger: */
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)value << 3) | 1));
			GIV(stackPointer) = sp;
			goto l1;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		goto l1;
	}
	if (fmt == (sixtyFourBitIndexableFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 3;
		if ((((usqInt)index)) < numSlots) {
			oop = signed64BitIntegerFor(long64At((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 3))))));

			/* begin methodReturnValue: */
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
			GIV(stackPointer) = sp;
			goto l1;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		goto l1;
	}
	if (fmt >= (firstLongFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 2;
		if ((((usqInt)index)) < numSlots) {
			value = long32At((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 2)))));
			if (value > 0x7FFFFFFF) {
				value -= 0x100000000LL;
			}

			/* begin methodReturnInteger: */
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)value << 3) | 1));
			GIV(stackPointer) = sp;
			goto l1;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		goto l1;
	}
	if ((fmt <= 5 /* lastPointerFormat */)
	 && (/* isIndexableFormat: */
		(fmt >= (arrayFormat()))
	 && ((fmt <= (weakArrayFormat()))
	 || (fmt >= (sixtyFourBitIndexableFormat()))))) {
		/* begin numSlotsOf: */
		assert((classIndexOf(rcvr)) > (isForwardedObjectClassIndexPun()));
		numSlots = (((numSlotsUsqInt = byteAt((void *)(rcvr + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(rcvr - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlotsUsqInt);
		if (fmt == (arrayFormat())) {
			if ((((usqInt)index)) < numSlots) {
				/* begin methodReturnValue: */
				assert(!((failed())));
				longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),longAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord())))))));
				GIV(stackPointer) = sp;
				goto l1;
			}
		}

		/* begin numFixedSlotsOf: */
		classPointer = fetchClassOfNonImm(rcvr);
		classFormat = ((longAt((void *)((classPointer + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3);
		numFixed = classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
		if ((((index + 1) >= numFixed) && ((index + 1) <= numSlots))) {
			/* begin methodReturnValue: */
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),longAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)((index + numFixed)) << (shiftForWord())))))));
			GIV(stackPointer) = sp;
			goto l1;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		goto l1;
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadReceiver;
	/* end primitiveSpurIntegerAt */
l1:;
}