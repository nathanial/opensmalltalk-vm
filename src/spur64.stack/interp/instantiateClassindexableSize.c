/* Extracted from interp.c:27144 (function instantiateClassindexableSize). */

/*	Allocate an instance of a variable class, excepting CompiledMethod. */

	/* Spur64BitMemoryManager>>#instantiateClass:indexableSize: */

sqInt
instantiateClassindexableSize(sqInt classObj, usqInt nElements)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classFormat;
    sqInt classIndex;
    sqInt err;
    sqInt fillValue;
    sqInt hash;
    sqInt instSpec;
    sqInt newObj;
    usqInt newObjUsqInt;
    usqInt numBytes;
    usqInt numSlots;
    usqInt p;
    usqInt toDoLimit;

	classFormat = ((longAt((void *)((classObj + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3);
	instSpec = (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
	classIndex = (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask());
	fillValue = 0;
	switch (instSpec) {
	case arrayFormat():
		numSlots = nElements;
		fillValue = nilObj;
		break;
	case indexablePointersFormat():
	case weakArrayFormat():
		numSlots = (classFormat & ((1U << (fixedFieldsFieldWidth())) - 1)) + nElements;
		fillValue = nilObj;
		break;
	case sixtyFourBitIndexableFormat():
		numSlots = nElements;
		break;
	case firstLongFormat():
		if ((classIndex == ClassFloatCompactIndex)
		 && (nElements != 2)) {
			/* primitiveFailFor: */
			primFailCode = PrimErrBadReceiver;
			return null;
		}
		numSlots = (nElements + 1) / 2;
		instSpec += nElements & 1;
		break;
	case firstShortFormat():
		numSlots = (nElements + 3) / 4;
		instSpec += (4 - nElements) & 3;
		break;
	case firstByteFormat():
		numSlots = (nElements + 7) / 8;
		instSpec += (8 - nElements) & 7;
		break;
	default:
		if ((nElements != 0)
		 || (instSpec > 5 /* lastPointerFormat */)) {
			return null;
		}
		numSlots = classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
		fillValue = nilObj;
	}

	/* non-indexable
	   Some Squeak images include funky fixed subclasses of abstract variable
	   superclasses. e.g. DirectoryEntry as a subclass of ArrayedCollection.
	   The (Threaded)FFIPlugin expects to be able to instantiate ExternalData via
	   this method.
	   Hence allow fixed classes to be instantiated here iff nElements = 0. */
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
			primFailCode = -classIndex;
			return null;
		}
	}
	if (numSlots > ((1U << (fixedFieldsFieldWidth())) - 1)) {
		if (numSlots > (0x10000000000LL)) {
			/* primitiveFailFor: */
			primFailCode = PrimErrUnsupported;
			return null;
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
			newObjUsqInt = freeStart + BaseHeaderSize;
			numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
		}
		else {
			newObjUsqInt = freeStart;
			numBytes = BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
		}
		if ((freeStart + numBytes) > scavengeThreshold) {
			if (numSlots <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
				if (!needGCFlag) {
					/* begin scheduleScavenge */
					needGCFlag = 1;
					forceInterruptCheck();
				}
			}
			newObj = allocateSlotsInOldSpacebytesformatclassIndex(numSlots, numBytes, instSpec, classIndex);
			goto l1;
		}
		if (numSlots >= (numSlotsMask())) {
			longAtput((void *)(freeStart),((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + numSlots);
			longAtput((void *)(newObjUsqInt),((((((usqLong) (numSlotsMask()))) << (numSlotsFullShift()))) + ((((usqInt)(instSpec) << (formatShift()))))) + classIndex);
		}
		else {
			longAtput((void *)(newObjUsqInt),((((((usqLong) numSlots)) << (numSlotsFullShift()))) + ((((usqInt)(instSpec) << (formatShift()))))) + classIndex);
		}

		/* for header parsing we put a saturated slot count in the prepended overflow size word */
		assert((numBytes % (allocationUnit())) == 0);
		assert((newObjUsqInt % (allocationUnit())) == 0);
		freeStart += numBytes;
		newObj = newObjUsqInt;
		/* end allocateSlots:format:classIndex: */
l1:;
	}
	if (newObj) {
		/* begin fillObj:numSlots:with: */
		assert(oopisLessThan(((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1, addressAfter(newObj)));
		toDoLimit = ((usqInt)(((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1));
		for (p = (((usqInt)(newObj + BaseHeaderSize))); p <= toDoLimit; p += 8 /* allocationUnit */) {
			longAtput((void *)(p),fillValue);
		}
	}
	return newObj;
}