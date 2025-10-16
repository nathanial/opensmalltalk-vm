/* Extracted from interp.c:15670 (function primitiveFloatArrayAtPut). */

static void
primitiveFloatArrayAtPut(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    float aFloat;
    usqLong bits;
    sqInt fmt;
    sqInt index;
    sqInt numSlots;
    sqInt rcvr;
    double result;
    char *sp;
    sqInt tagBits;
    sqLong value;
    sqInt valueOop;

	/* begin primitiveSpurFloatArrayAtPut */
	valueOop = longAt(GIV(stackPointer));
	index = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	rcvr = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	if (!((/* isFloatInstance: */
			((tagBits = valueOop & (tagMask()))
				? tagBits == (smallFloatTag())
				: ((longAt((void *)(valueOop))) & (classIndexMask())) == ClassFloatCompactIndex))
		 && ((((index) & 7) == 1)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		goto l1;
	}
	if (((rcvr & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		goto l1;
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
		goto l1;
	}
	fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
	index = ((index >> 3)) - 1;
	if (fmt == (sixtyFourBitIndexableFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 3;
		if ((((usqInt)index)) < numSlots) {
			value = floatValueBitsOf(valueOop);

			/* begin storeLong64:ofObject:withValue: */
			long64Atput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 3)))),value);
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),valueOop);
			GIV(stackPointer) = sp;
			goto l1;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		goto l1;
	}

	/* N.B. Currently we simply truncate to 32-bits, which matches the behavior of the FloatArrayPlugin.
	   Maybe we should validate and range check. */
	if ((fmt >= (firstLongFormat()))
	 && (fmt <= ((firstLongFormat()) + 1))) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 2;
		if ((((usqInt)index)) < numSlots) {
			/* begin noFailFloatValueOf: */
			assert(isFloatInstance(valueOop));
			if (valueOop & (tagMask())) {
				/* begin smallFloatValueOf: */
				/* begin smallFloatBitsOf: */
				assert(isImmediateFloat(valueOop));
				bits = ((((usqInt)valueOop))) >> (numTagBits());
				if (bits > 1) {
					bits += (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
				}

				/* a.k.a. ~= +/-0.0 */
				bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
				memcpy((&result), (&bits), sizeof(result));
			}
			else {
				fetchFloatAtinto(valueOop + BaseHeaderSize, result);
			}
			aFloat = result;

			/* begin storeFloat32:ofObject:withValue: */
			singleFloatAtput((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 2)))),aFloat);
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),valueOop);
			GIV(stackPointer) = sp;
			goto l1;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		goto l1;
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadReceiver;
	/* end primitiveSpurFloatArrayAtPut */
l1:;
}