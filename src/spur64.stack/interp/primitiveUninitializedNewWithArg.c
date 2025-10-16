/* Extracted from interp.c:25424 (function primitiveUninitializedNewWithArg). */

static void
primitiveUninitializedNewWithArg(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classFormat;
    sqInt classIndex;
    sqInt classObj;
    sqInt err;
    sqInt hash;
    sqInt instSpec;
    sqInt instSpecSqInt;
    sqInt newObj;
    usqInt newObjUsqInt;
    usqInt numBytes;
    usqInt numSlots;
    sqInt obj;
    sqInt reasonCode;
    usqIntptr_t size;
    char *sp;
    sqInt value;


	/* For the mirror prims check that the class obj is actually a valid class. */

	/* begin positiveMachineIntegerValueOf: */
	if (((((longAt(GIV(stackPointer)))) & 7) == 1)) {
		value = ((longAt(GIV(stackPointer))) >> 3);
		if (value < 0) {
			/* begin primitiveFail */
			if (!GIV(primFailCode)) {
				GIV(primFailCode) = 1;
			}
			size = null;
			goto l1;
		}
		size = value;
		goto l1;
	}

	/* don't inline the rare case */
	size = positiveMachineIntegerValueOfObj(longAt(GIV(stackPointer)));
	/* end positiveMachineIntegerValueOf: */
l1:
	if (GIV(primFailCode)) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}

	/* positiveMachineIntegerValueOf: succeeds only for non-negative integers. */
	classObj = longAt(GIV(stackPointer) + (1 * BytesPerWord));

	/* begin instantiateUninitializedClass:indexableSize: */
	classFormat = ((longAt((void *)((classObj + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3);
	instSpecSqInt = (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
	classIndex = (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask());
	assert(isPureBitsFormat(instSpecSqInt));
	switch (instSpecSqInt) {
	case sixtyFourBitIndexableFormat():
		numSlots = size;
		break;
	case firstLongFormat():
		if ((classIndex == ClassFloatCompactIndex)
		 && (size != 2)) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrBadReceiver;
			obj = null;
			goto l3;
		}
		numSlots = (size + 1) / 2;
		instSpecSqInt += size & 1;
		break;
	case firstShortFormat():
		numSlots = (size + 3) / 4;
		instSpecSqInt += (4 - size) & 3;
		break;
	case firstByteFormat():
		numSlots = (size + 7) / 8;
		instSpecSqInt += (8 - size) & 7;
		break;
	default:
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		obj = null;
		goto l3;
	}

	/* not bits indexable */
	if (!classIndex) {
		/* begin ensureBehaviorHash: */
		assert(addressCouldBeClassObj(classObj));

		/* eem 12/28/2021 the above asserft is too weak (and only an assert) */
		classIndex = ((hash = (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask()))
					? hash
					: (objCouldBeClassObj(classObj)
							? ((err = enterIntoClassTable(classObj))
									? -err
									: (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask()))
							: -PrimErrBadReceiver));
		if (classIndex < 0) {
			/* primitiveFailFor: */
			GIV(primFailCode) = -classIndex;
			obj = null;
			goto l3;
		}
	}
	if (numSlots > ((1U << (fixedFieldsFieldWidth())) - 1)) {
		if (numSlots > (0x10000000000LL)) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrUnsupported;
			obj = null;
			goto l3;
		}
		newObj = allocateSlotsInOldSpacebytesformatclassIndex(numSlots, /* objectBytesForSlots: */
				(numSlots
					? ((numSlots << (shiftForWord()))) + ((numSlots >= (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize))
					: 8 /* allocationUnit */ + BaseHeaderSize), instSpecSqInt, classIndex);
	}
	else {
		/* begin allocateSlots:format:classIndex: */
		if (numSlots >= (numSlotsMask())) {
			if (((numSlots) >> 56) > 0) {
				newObj = null;
				goto l2;
			}
			newObjUsqInt = GIV(freeStart) + BaseHeaderSize;
			numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
		}
		else {
			newObjUsqInt = GIV(freeStart);
			numBytes = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
		}
		if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
			if (numSlots <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
				if (!GIV(needGCFlag)) {
					/* begin scheduleScavenge */
					GIV(needGCFlag) = 1;
					forceInterruptCheck();
				}
			}
			newObj = allocateSlotsInOldSpacebytesformatclassIndex(numSlots, numBytes, instSpecSqInt, classIndex);
			goto l2;
		}
		if (numSlots >= (numSlotsMask())) {
			longAtput((void *)(GIV(freeStart)),((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + numSlots);
			longAtput((void *)(newObjUsqInt),((((((usqLong) (numSlotsMask()))) << (numSlotsFullShift()))) + ((((usqInt)(instSpecSqInt) << (formatShift()))))) + classIndex);
		}
		else {
			longAtput((void *)(newObjUsqInt),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(instSpecSqInt) << (formatShift()))))) + classIndex);
		}

		/* for header parsing we put a saturated slot count in the prepended overflow size word */
		assert((numBytes % (allocationUnit())) == 0);
		assert((newObjUsqInt % (allocationUnit())) == 0);
		GIV(freeStart) += numBytes;
		newObj = newObjUsqInt;
		/* end allocateSlots:format:classIndex: */
l2:;
	}
	obj = newObj;
	/* end instantiateUninitializedClass:indexableSize: */
l3:
	if (obj) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),obj);
		GIV(stackPointer) = sp;
	}
	else {
		instSpec = (((usqInt)((((longAt((void *)(((longAt(GIV(stackPointer) + (1 * BytesPerWord))) + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)))) >> (fixedFieldsFieldWidth())) & (formatMask());
		reasonCode = ((/* isIndexableFormat: */
				(instSpec >= (arrayFormat()))
			 && ((instSpec <= (weakArrayFormat()))
			 || (instSpec >= (sixtyFourBitIndexableFormat()))))
			 && (!(instSpec >= (firstCompiledMethodFormat())))
					? PrimErrNoMemory
					: PrimErrBadReceiver);

		/* begin primitiveFailFor: */
		GIV(primFailCode) = reasonCode;
	}
}