/* Extracted from interp.c:29935 (function allInstancesOf). */

static NoDbgRegParms sqInt
allInstancesOf(sqInt aClass)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt actualIndex;
    usqInt bytes;
    sqInt classIndex;
    sqInt classOrNil;
    sqInt count;
    sqInt countSqInt;
    sqInt expectedIndex;
    sqInt expectedIndexSqInt;
    sqInt followingWord;
    usqInt followingWordAddress;
    sqInt freeChunk;
    sqInt i;
    sqInt index;
    sqInt iSqInt;
    sqInt j;
    usqInt limit;
    usqInt newObj;
    usqInt numBytes;
    usqInt numSlots;
    sqInt objOop;
    sqInt page;
    sqInt prevObj;
    sqInt prevPrevObj;
    sqInt ptr;
    sqInt ptrSqInt;
    sqInt smallObj;
    usqInt start;
    usqInt startUsqInt;
    sqInt toDoLimit;
    sqInt valuePointer;

	classIndex = (long32At((void *)(aClass + 4))) & (identityHashHalfWordMask());
	if (!classIndex) {
		/* begin allocateSlots:format:classIndex: */
		newObj = GIV(freeStart);
		numBytes = BaseHeaderSize + 8 /* allocationUnit */;
		if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
			if (0 <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
				if (!GIV(needGCFlag)) {
					/* begin scheduleScavenge */
					GIV(needGCFlag) = 1;
					forceInterruptCheck();
				}
			}
			freeChunk = allocateSlotsInOldSpacebytesformatclassIndex(0, numBytes, arrayFormat(), ClassArrayCompactIndex);
			goto l1;
		}
		longAtput((void *)(newObj),((((((usqLong) 0)) << (numSlotsFullShift()))) + ((((usqInt)((arrayFormat())) << (formatShift()))))) + ClassArrayCompactIndex);

		/* for header parsing we put a saturated slot count in the prepended overflow size word */
		assert((numBytes % (allocationUnit())) == 0);
		assert((newObj % (allocationUnit())) == 0);
		GIV(freeStart) += numBytes;
		freeChunk = newObj;
		/* end allocateSlots:format:classIndex: */
l1:
		return freeChunk;
	}

	/* N.B. Does /not/ update totalFreeOldSpace */
	freeChunk = allocateLargestFreeChunk();
	start = freeChunk + BaseHeaderSize;
	limit = addressAfter(freeChunk);

	/* begin isClassAtUniqueIndex: */
	expectedIndex = (long32At((void *)(aClass + 4))) & (identityHashHalfWordMask());
	for (iSqInt = 0; iSqInt < GIV(numClassTablePages); iSqInt += 1) {
		page = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))));
		toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
		for (j = 0; j <= toDoLimit; j += 1) {
			classOrNil = longAt((void *)((page + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))));
			if (classOrNil != GIV(nilObj)) {
				index = ((((usqInt)(iSqInt) << (classTableMajorIndexShift())))) + j;
				if ((classOrNil == aClass)
				 && ((index != expectedIndex)
				 && (index > (lastClassIndexPun())))) {
					goto l4;
				}
			}
		}
	}

	/* begin uniqueIndex:allInstancesInto:limit:resultsInto: */
	countSqInt = 0;
	ptrSqInt = start;

	/* begin allHeapEntitiesDo: */
	/* begin allOldSpaceEntitiesDo: */
	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(GIV(nilObj)));
	prevPrevObj = (prevObj = null);
	objOop = GIV(nilObj);
	while (1) {
		assert((objOop % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOop, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objOop))) != 0);
		if (((longAt((void *)(objOop))) & (classIndexMask())) > (lastClassIndexPun())) {
			if (((longAt((void *)(objOop))) & (classIndexMask())) == classIndex) {
				countSqInt += 1;
				if (ptrSqInt < limit) {
					longAtput((void *)(ptrSqInt),objOop);
					ptrSqInt += BytesPerOop;
				}
			}
		}
		else {
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			objOop = GIV(endOfMemory);
			goto l10;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l10:;
	}

	/* begin allNewSpaceEntitiesDo: */
	prevPrevObj = (prevObj = null);

	/* After a scavenge eden is empty, futureSpace is empty, and all newSpace objects are
	   in pastSpace.  Objects are allocated in eden.  So enumerate only pastSpace and eden. */
	assert((((GIV(pastSpace)).start)) < (((GIV(eden)).start)));
	startUsqInt = /* startAddressForBridgedHeapEnumeration */
			(GIV(pastSpaceStart) > (((GIV(pastSpace)).start))
				? ((GIV(pastSpace)).start)
				: (GIV(freeStart) > (((GIV(eden)).start))
						? ((GIV(eden)).start)
						: GIV(oldSpaceStart)));
	if (startUsqInt > GIV(freeStart)) {
		goto l9;
	}

	/* begin bridgePastSpaceAndEden */
	if (GIV(pastSpaceStart) < (((GIV(eden)).start))) {
		if ((GIV(pastSpaceStart) + BaseHeaderSize) == (((GIV(eden)).start))) {
			hackSlimBridgeToat(objectStartingAt(((GIV(eden)).start)), GIV(pastSpaceStart));

			/* And carefully check the assumption */
			assert((objectAfterMaybeSlimBridgelimit(objectInPastSpaceBefore(GIV(pastSpaceStart)), GIV(nilObj))) == (objectStartingAt(((GIV(eden)).start))));
		}
		else {
			initSegmentBridgeWithBytesat((((GIV(eden)).start)) - GIV(pastSpaceStart), GIV(pastSpaceStart));
		}
	}

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(startUsqInt + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? startUsqInt + BaseHeaderSize
				: startUsqInt);
	while (oopisLessThan(objOop, GIV(freeStart))) {
		if (((longAt((void *)(objOop))) & (classIndexMask())) > (lastClassIndexPun())) {
			if (((longAt((void *)(objOop))) & (classIndexMask())) == classIndex) {
				countSqInt += 1;
				if (ptrSqInt < limit) {
					longAtput((void *)(ptrSqInt),objOop);
					ptrSqInt += BytesPerOop;
				}
			}
		}
		else {
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(freeStart))) {
			objOop = GIV(freeStart);
			goto l8;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOop, GIV(oldSpaceStart)))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l8:;
	}
	/* end allNewSpaceEntitiesDo: */
l9:

	/* continue enumerating even if no room so as to unmark all objects. */
	count = countSqInt;
	ptr = ptrSqInt;
	goto l3;
l4:

	/* begin ambiguousClass:allInstancesInto:limit:resultsInto: */
	countSqInt = 0;
	ptrSqInt = start;
	expectedIndex = (long32At((void *)(aClass + 4))) & (identityHashHalfWordMask());

	/* begin allHeapEntitiesDo: */
	/* begin allOldSpaceEntitiesDo: */
	/* begin allOldSpaceEntitiesFrom:do: */
	assert(isOldObject(GIV(nilObj)));
	prevPrevObj = (prevObj = null);
	objOop = GIV(nilObj);
	while (1) {
		assert((objOop % (allocationUnit())) == 0);
		if (!(oopisLessThan(objOop, GIV(endOfMemory)))) break;
		assert((long64At((void *)(objOop))) != 0);
		if (((longAt((void *)(objOop))) & (classIndexMask())) > (lastClassIndexPun())) {
			actualIndex = (longAt((void *)(objOop))) & (classIndexMask());
			if ((classOrNilAtIndex(actualIndex)) == aClass) {
				if (actualIndex != expectedIndex) {
					/* begin setClassIndexOf:to: */
					assert(((expectedIndex >= 0) && (expectedIndex <= (classIndexMask()))));
					longAtput((void *)(objOop),((longAt((void *)(objOop))) & (~(usqIntptr_t)(classIndexMask()))) + expectedIndex);
				}
				countSqInt += 1;
				if (ptrSqInt < limit) {
					longAtput((void *)(ptrSqInt),objOop);
					ptrSqInt += BytesPerOop;
				}
			}
		}
		else {
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfter:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(endOfMemory))) {
			objOop = GIV(endOfMemory);
			goto l7;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? followingWordAddress + BaseHeaderSize
					: followingWordAddress);
		/* end objectAfter:limit: */
l7:;
	}

	/* begin allNewSpaceEntitiesDo: */
	prevPrevObj = (prevObj = null);

	/* After a scavenge eden is empty, futureSpace is empty, and all newSpace objects are
	   in pastSpace.  Objects are allocated in eden.  So enumerate only pastSpace and eden. */
	assert((((GIV(pastSpace)).start)) < (((GIV(eden)).start)));
	startUsqInt = /* startAddressForBridgedHeapEnumeration */
			(GIV(pastSpaceStart) > (((GIV(pastSpace)).start))
				? ((GIV(pastSpace)).start)
				: (GIV(freeStart) > (((GIV(eden)).start))
						? ((GIV(eden)).start)
						: GIV(oldSpaceStart)));
	if (startUsqInt > GIV(freeStart)) {
		goto l6;
	}

	/* begin bridgePastSpaceAndEden */
	if (GIV(pastSpaceStart) < (((GIV(eden)).start))) {
		if ((GIV(pastSpaceStart) + BaseHeaderSize) == (((GIV(eden)).start))) {
			hackSlimBridgeToat(objectStartingAt(((GIV(eden)).start)), GIV(pastSpaceStart));

			/* And carefully check the assumption */
			assert((objectAfterMaybeSlimBridgelimit(objectInPastSpaceBefore(GIV(pastSpaceStart)), GIV(nilObj))) == (objectStartingAt(((GIV(eden)).start))));
		}
		else {
			initSegmentBridgeWithBytesat((((GIV(eden)).start)) - GIV(pastSpaceStart), GIV(pastSpaceStart));
		}
	}

	/* begin objectStartingAt: */
	numSlots = byteAt((void *)(startUsqInt + (numSlotsFieldByteOffset())));
	objOop = (numSlots == (numSlotsMask())
				? startUsqInt + BaseHeaderSize
				: startUsqInt);
	while (oopisLessThan(objOop, GIV(freeStart))) {
		if (((longAt((void *)(objOop))) & (classIndexMask())) > (lastClassIndexPun())) {
			actualIndex = (longAt((void *)(objOop))) & (classIndexMask());
			if ((classOrNilAtIndex(actualIndex)) == aClass) {
				if (actualIndex != expectedIndex) {
					/* begin setClassIndexOf:to: */
					assert(((expectedIndex >= 0) && (expectedIndex <= (classIndexMask()))));
					longAtput((void *)(objOop),((longAt((void *)(objOop))) & (~(usqIntptr_t)(classIndexMask()))) + expectedIndex);
				}
				countSqInt += 1;
				if (ptrSqInt < limit) {
					longAtput((void *)(ptrSqInt),objOop);
					ptrSqInt += BytesPerOop;
				}
			}
		}
		else {
		}
		prevPrevObj = prevObj;
		prevObj = objOop;

		/* begin objectAfterMaybeSlimBridge:limit: */
		followingWordAddress = addressAfter(objOop);
		if (oopisGreaterThanOrEqualTo(followingWordAddress, GIV(freeStart))) {
			objOop = GIV(freeStart);
			goto l5;
		}
		followingWord = longAt((void *)(followingWordAddress));
		objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) == (numSlotsMask())
					? ((oopisLessThan(objOop, GIV(oldSpaceStart)))
					 && ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
							? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
							: followingWordAddress + BaseHeaderSize)
					: followingWordAddress);
		/* end objectAfterMaybeSlimBridge:limit: */
l5:;
	}
	/* end allNewSpaceEntitiesDo: */
l6:

	/* continue enumerating even if no room so as to unmark all objects and/or normalize class indices. */

	/* begin purgeDuplicateClassTableEntriesFor: */
	expectedIndexSqInt = (long32At((void *)(aClass + 4))) & (identityHashHalfWordMask());
	for (iSqInt = 0; iSqInt < GIV(numClassTablePages); iSqInt += 1) {
		page = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(iSqInt) << (shiftForWord()))))));
		toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
		for (j = 0; j <= toDoLimit; j += 1) {
			classOrNil = longAt((void *)((page + BaseHeaderSize) + ((((usqInt)(j) << (shiftForWord()))))));
			if (classOrNil != GIV(nilObj)) {
				index = ((((usqInt)(iSqInt) << (classTableMajorIndexShift())))) + j;
				if ((classOrNil == aClass)
				 && ((index != expectedIndexSqInt)
				 && (index > (lastClassIndexPun())))) {
					classAtIndexput(index, GIV(nilObj));
					if (index < GIV(classTableIndex)) {
						GIV(classTableIndex) = index;
					}
				}
			}
		}
	}

	/* classTableIndex must never index the first page, which is reserved for classes known to the VM. */
	assert(GIV(classTableIndex) >= (1U << (classTableMajorIndexShift())));
	count = countSqInt;
	ptr = ptrSqInt;
l3:
	assert(isEmptyObjStack(GIV(markStack)));
	assert(isEmptyObjStack(GIV(weaklingStack)));
	if ((count > ((ptr - start) / BytesPerOop))
	 || ((limit != ptr)
	 && ((limit - ptr) <= 8 /* allocationUnit */))) {
		freeChunkWithBytesat(bytesInBody(freeChunk), /* startOfObject: */
			((byteAt((void *)(freeChunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
				? freeChunk - BaseHeaderSize
				: freeChunk));

		/* begin checkFreeSpace: */
		assert(bitsSetInFreeSpaceMaskForAllFreeLists());
		assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
		if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) == (GCCheckFreeSpace | GCModeFull))) {
			runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
		}
		return (((usqInt)count << 3) | 1);
	}

	/* not enough room
	   can't split a single word */
	if (count < (numSlotsMask())) {
		/* begin allocateSlots:format:classIndex: */
		if (count >= (numSlotsMask())) {
			if ((((usqInt)(count)) >> 56) > 0) {
				smallObj = null;
				goto l2;
			}
			newObj = GIV(freeStart) + BaseHeaderSize;
			numBytes = (BaseHeaderSize + BaseHeaderSize) + (count * BytesPerOop);
		}
		else {
			newObj = GIV(freeStart);
			numBytes = BaseHeaderSize + ((count < 1
		? 8 /* allocationUnit */
		: count * BytesPerOop));
		}
		if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
			if (count <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
				if (!GIV(needGCFlag)) {
					/* begin scheduleScavenge */
					GIV(needGCFlag) = 1;
					forceInterruptCheck();
				}
			}
			smallObj = allocateSlotsInOldSpacebytesformatclassIndex(count, numBytes, arrayFormat(), ClassArrayCompactIndex);
			goto l2;
		}
		if (count >= (numSlotsMask())) {
			longAtput((void *)(GIV(freeStart)),((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) + count);
			longAtput((void *)(newObj),((((((usqLong) (numSlotsMask()))) << (numSlotsFullShift()))) + ((((usqInt)((arrayFormat())) << (formatShift()))))) + ClassArrayCompactIndex);
		}
		else {
			longAtput((void *)(newObj),((((((usqLong) count)) << (numSlotsFullShift()))) + ((((usqInt)((arrayFormat())) << (formatShift()))))) + ClassArrayCompactIndex);
		}

		/* for header parsing we put a saturated slot count in the prepended overflow size word */
		assert((numBytes % (allocationUnit())) == 0);
		assert((newObj % (allocationUnit())) == 0);
		GIV(freeStart) += numBytes;
		smallObj = newObj;
		/* end allocateSlots:format:classIndex: */
l2:
		for (i = 0; i < count; i += 1) {
			valuePointer = longAt((void *)((freeChunk + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));

			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(smallObj))
			 && (!(isForwarded(smallObj))));
			assert(validStorePointerUncheckedArgs(i, smallObj, valuePointer));
			longAtput((void *)((smallObj + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),valuePointer);
		}
		freeChunkWithBytesat(bytesInBody(freeChunk), /* startOfObject: */
			((byteAt((void *)(freeChunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
				? freeChunk - BaseHeaderSize
				: freeChunk));
		beRootIfOld(smallObj);

		/* begin checkFreeSpace: */
		assert(bitsSetInFreeSpaceMaskForAllFreeLists());
		assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
		if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) == (GCCheckFreeSpace | GCModeFull))) {
			runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
		}
		return smallObj;
	}
	bytes = (BaseHeaderSize + BaseHeaderSize) + (count * BytesPerOop);
	start = /* startOfObject: */
			((byteAt((void *)(freeChunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
				? freeChunk - BaseHeaderSize
				: freeChunk);
	freeChunkWithBytesat((limit - start) - bytes, start + bytes);
	GIV(totalFreeOldSpace) -= bytes;

	/* begin rawOverflowSlotsOf:put: */
	longAtput((void *)(freeChunk - BaseHeaderSize),((((usqInt)((numSlotsMask())) << 56))) + count);

	/* begin set:classIndexTo:formatTo: */
	assert(((ClassArrayCompactIndex >= 0) && (ClassArrayCompactIndex <= (classIndexMask()))));
	assert((((arrayFormat()) >= 0) && ((arrayFormat()) <= (formatMask()))));
	longAtput((void *)(freeChunk),((longAt((void *)(freeChunk))) & (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) + (classIndexMask())))) + (ClassArrayCompactIndex + ((((usqInt)((arrayFormat())) << (formatShift()))))));

	/* begin possibleRootStoreInto: */
	if (!((byteAt((void *)(freeChunk + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
		remember(freeChunk);
	}

	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) == (GCCheckFreeSpace | GCModeFull))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}
	runLeakCheckerFor(GCModeFull);
	return freeChunk;
}