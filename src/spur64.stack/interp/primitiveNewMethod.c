/* Extracted from interp.c:19459 (function primitiveNewMethod). */

static void
primitiveNewMethod(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bytecodeCount;
    sqInt class;
    sqInt classFormat;
    sqInt classIndex;
    sqInt err;
    sqInt hash;
    sqInt header;
    sqInt i;
    sqInt instSpec;
    usqInt literalCount;
    sqInt newObj;
    usqInt newObjUsqInt;
    usqInt numBytes;
    usqInt numSlots;
    usqInt p;
    sqInt reasonCode;
    sqInt size;
    char *sp;
    sqInt theMethod;
    usqInt toDoLimit;

	header = longAt(GIV(stackPointer));
	bytecodeCount = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (!(((((header) & 7) == 1))
		 && (((((bytecodeCount) & 7) == 1))
		 && (((bytecodeCount = (bytecodeCount >> 3))) >= 0)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	class = longAt(GIV(stackPointer) + (2 * BytesPerWord));

	/* begin literalCountOfMethodHeader: */
	assert((((header) & 7) == 1));
	literalCount = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
	size = ((literalCount + LiteralStart) * BytesPerOop) + bytecodeCount;

	/* begin instantiateCompiledMethodClass:indexableSize: */
	classFormat = ((longAt((void *)((class + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3);
	instSpec = (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
	if (instSpec != (firstCompiledMethodFormat())) {
		theMethod = null;
		goto l2;
	}
	numSlots = (size + 7) / 8;
	instSpec += (8 - size) & 7;

	/* begin ensureBehaviorHash: */
	assert(addressCouldBeClassObj(class));

	/* eem 12/28/2021 the above asserft is too weak (and only an assert) */
	classIndex = ((hash = (long32At((void *)(class + 4))) & (identityHashHalfWordMask()))
				? hash
				: (objCouldBeClassObj(class)
						? ((err = enterIntoClassTable(class))
								? -err
								: (long32At((void *)(class + 4))) & (identityHashHalfWordMask()))
						: -PrimErrBadReceiver));
	if (classIndex < 0) {
		/* primitiveFailFor: */
		GIV(primFailCode) = -classIndex;
		theMethod = null;
		goto l2;
	}
	if (numSlots > ((1U << (fixedFieldsFieldWidth())) - 1)) {
		if (numSlots > (0x10000000000LL)) {
			theMethod = null;
			goto l2;
		}
		newObj = allocateSlotsInOldSpacebytesformatclassIndex(numSlots, /* objectBytesForSlots: */
				(numSlots
					? ((numSlots << (shiftForWord()))) + ((numSlots >= (numSlotsMask())
		? BaseHeaderSize + BaseHeaderSize
		: BaseHeaderSize))
					: 8 /* allocationUnit */ + BaseHeaderSize), instSpec, classIndex);
	}
	else {
		/* begin allocateSlots:format:classIndex: */
		if (numSlots >= (numSlotsMask())) {
			if (((numSlots) >> 56) > 0) {
				newObj = null;
				goto l1;
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
			newObj = allocateSlotsInOldSpacebytesformatclassIndex(numSlots, numBytes, instSpec, classIndex);
			goto l1;
		}
		if (numSlots >= (numSlotsMask())) {
			longAtput((void *)(GIV(freeStart)),((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + numSlots);
			longAtput((void *)(newObjUsqInt),((((((usqLong) (numSlotsMask()))) << (numSlotsFullShift()))) + ((((usqInt)(instSpec) << (formatShift()))))) + classIndex);
		}
		else {
			longAtput((void *)(newObjUsqInt),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(instSpec) << (formatShift()))))) + classIndex);
		}

		/* for header parsing we put a saturated slot count in the prepended overflow size word */
		assert((numBytes % (allocationUnit())) == 0);
		assert((newObjUsqInt % (allocationUnit())) == 0);
		GIV(freeStart) += numBytes;
		newObj = newObjUsqInt;
		/* end allocateSlots:format:classIndex: */
l1:;
	}
	if (newObj) {
		/* begin fillObj:numSlots:with: */
		assert(oopisLessThan(((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1, addressAfter(newObj)));
		toDoLimit = ((usqInt)(((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1));
		for (p = (((usqInt)(newObj + BaseHeaderSize))); p <= toDoLimit; p += 8 /* allocationUnit */) {
			longAtput((void *)(p),0);
		}
	}
	theMethod = newObj;
	/* end instantiateCompiledMethodClass:indexableSize: */
l2:
	if (!theMethod) {
		reasonCode = (isCompiledMethodFormat((((usqInt)((((longAt((void *)((class + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)))) >> (fixedFieldsFieldWidth())) & (formatMask()))
					? PrimErrNoMemory
					: PrimErrBadReceiver);

		/* begin primitiveFailFor: */
		GIV(primFailCode) = reasonCode;
		return;
	}

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(theMethod))
	 && (!(isForwarded(theMethod))));
	assert(validStorePointerUncheckedArgs(HeaderIndex, theMethod, header));
	longAtput((void *)((theMethod + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))),header);
	for (i = 1; i <= literalCount; i += 1) {
		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(theMethod))
		 && (!(isForwarded(theMethod))));
		assert(validStorePointerUncheckedArgs(i, theMethod, GIV(nilObj)));
		longAtput((void *)((theMethod + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),GIV(nilObj));
	}

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (2 * BytesPerWord)),theMethod);
	GIV(stackPointer) = sp;
}