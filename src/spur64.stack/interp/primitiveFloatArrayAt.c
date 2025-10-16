/* Extracted from interp.c:15534 (function primitiveFloatArrayAt). */

static void
primitiveFloatArrayAt(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    float aFloat;
    sqLong doubleBits;
    sqInt fmt;
    sqInt index;
    usqInt newFloatObj;
    usqInt newObj;
    usqInt numBytes;
    sqInt numSlots;
    sqInt numSlotsSqInt;
    sqInt oop;
    usqLong rawFloatBits;
    sqInt rcvr;
    usqLong rot;
    char *sp;

	/* begin primitiveSpurFloatArrayAt */
	index = longAt(GIV(stackPointer));
	rcvr = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (!((((index) & 7) == 1))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		goto l3;
	}
	if (((rcvr & (tagMask())) != 0)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		goto l3;
	}
	fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
	index = ((index >> 3)) - 1;
	if (fmt == (sixtyFourBitIndexableFormat())) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 3;
		if ((((usqInt)index)) < numSlots) {
			doubleBits = long64At((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 3)))));

			/* begin floatObjectOfBits: */
			if (isSmallFloatValueBits(doubleBits)) {
				rawFloatBits = doubleBits;

				/* begin smallFloatObjectOfBits: */
				assert(isSmallFloatValueBits(rawFloatBits));
				rot = (((rawFloatBits) >> 0x3F) & 1) + ((rawFloatBits << 1));
				if (rot > 1) {
					rot -= (((usqInt)((smallFloatExponentOffset())) << ((smallFloatMantissaBits()) + 1)));
					assert(rot > 0);
				}

				/* a.k.a. ~= +/-0.0 */
				oop = ((rot << (numTagBits()))) + (smallFloatTag());
				goto l2;
			}
			numSlotsSqInt = (sizeof(double)) / BytesPerOop;

			/* begin eeInstantiateSmallClassIndex:format:numSlots: */
			assert((numSlotsSqInt >= 0)
			 && ((knownClassAtIndex(ClassFloatCompactIndex)) != GIV(nilObj)));
			assert((firstLongFormat()) == (instSpecOfClass(knownClassAtIndex(ClassFloatCompactIndex))));

			/* begin allocateSmallNewSpaceSlots:format:classIndex: */
			assert(numSlotsSqInt < (numSlotsMask()));
			newObj = GIV(freeStart);
			numBytes = BaseHeaderSize + ((numSlotsSqInt < 1
		? 8 /* allocationUnit */
		: numSlotsSqInt * BytesPerOop));
			assert((numBytes % (allocationUnit())) == 0);
			assert((newObj % (allocationUnit())) == 0);
			if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
				if (!GIV(needGCFlag)) {
					/* begin scheduleScavenge */
					GIV(needGCFlag) = 1;
					forceInterruptCheck();
				}
				if ((GIV(freeStart) + numBytes) > (((GIV(eden)).limit))) {
					error("no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
					newFloatObj = 0;
					goto l1;
				}
			}
			long64Atput((void *)(newObj),((((((usqLong) numSlotsSqInt)) << (numSlotsFullShift()))) + ((((usqInt)((firstLongFormat())) << (formatShift()))))) + ClassFloatCompactIndex);
			GIV(freeStart) += numBytes;
			newFloatObj = newObj;
			/* end eeInstantiateSmallClassIndex:format:numSlots: */
l1:

			/* storeLong64:ofObject:withValue: */
			long64Atput((void *)((newFloatObj + BaseHeaderSize)),doubleBits);
			oop = newFloatObj;
			/* end floatObjectOfBits: */
l2:

			/* begin methodReturnValue: */
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
			GIV(stackPointer) = sp;
			goto l3;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		goto l3;
	}
	if ((fmt >= (firstLongFormat()))
	 && (fmt <= ((firstLongFormat()) + 1))) {
		numSlots = ((usqInt)((numBytesOf(rcvr)))) >> 2;
		if ((((usqInt)index)) < numSlots) {
			/* begin fetchFloat32:ofObject: */
			aFloat = singleFloatAt((void *)((rcvr + BaseHeaderSize) + ((((usqInt)(index) << 2)))));
			oop = floatObjectOf(aFloat);

			/* begin methodReturnValue: */
			assert(!((failed())));
			longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
			GIV(stackPointer) = sp;
			goto l3;
		}

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		goto l3;
	}

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadReceiver;
	/* end primitiveSpurFloatArrayAt */
l3:;
}