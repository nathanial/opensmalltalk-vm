/* Extracted from interp.c:56769 (function makePointwithxValueyValue). */

usqInt
makePointwithxValueyValue(sqInt xValue, sqInt yValue)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt newObj;
    usqInt numBytes;
    usqInt pointResult;

	/* begin eeInstantiatePoint */
	/* begin eeInstantiateSmallClassIndex:format:numSlots: */
	assert(((YIndex + 1) >= 0)
	 && ((knownClassAtIndex(ClassPointCompactIndex)) != GIV(nilObj)));
	assert((nonIndexablePointerFormat()) == (instSpecOfClass(knownClassAtIndex(ClassPointCompactIndex))));

	/* begin allocateSmallNewSpaceSlots:format:classIndex: */
	assert((YIndex + 1) < (numSlotsMask()));
	newObj = GIV(freeStart);
	numBytes = BaseHeaderSize + (((YIndex + 1) < 1
		? 8 /* allocationUnit */
		: (YIndex + 1) * BytesPerOop));
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
			pointResult = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) (YIndex + 1))) << (numSlotsFullShift()))) + ((((usqInt)((nonIndexablePointerFormat())) << (formatShift()))))) + ClassPointCompactIndex);
	GIV(freeStart) += numBytes;
	pointResult = newObj;
	/* end eeInstantiatePoint */
l1:

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(pointResult))
	 && (!(isForwarded(pointResult))));
	assert(validStorePointerUncheckedArgs(XIndex, pointResult, (((usqInt)xValue << 3) | 1)));
	longAtput((void *)((pointResult + BaseHeaderSize) + ((((usqInt)(XIndex) << (shiftForWord()))))),(((usqInt)xValue << 3) | 1));

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(pointResult))
	 && (!(isForwarded(pointResult))));
	assert(validStorePointerUncheckedArgs(YIndex, pointResult, (((usqInt)yValue << 3) | 1)));
	longAtput((void *)((pointResult + BaseHeaderSize) + ((((usqInt)(YIndex) << (shiftForWord()))))),(((usqInt)yValue << 3) | 1));
	return pointResult;
}