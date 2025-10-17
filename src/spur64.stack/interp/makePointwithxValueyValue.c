/* Extracted from interp.c:56769 (function makePointwithxValueyValue). */

/*	make a Point xValue@yValue.
	We know both will be integers so no value nor root checking is needed */

	/* StackInterpreter>>#makePointwithxValue:yValue: */

usqInt
makePointwithxValueyValue(sqInt xValue, sqInt yValue)
{
    usqInt newObj;
    usqInt numBytes;
    usqInt pointResult;

	/* begin eeInstantiatePoint */
	/* begin eeInstantiateSmallClassIndex:format:numSlots: */
	assert(((YIndex + 1) >= 0)
	 && ((knownClassAtIndex(ClassPointCompactIndex)) != nilObj));
	assert((nonIndexablePointerFormat()) == (instSpecOfClass(knownClassAtIndex(ClassPointCompactIndex))));

	/* begin allocateSmallNewSpaceSlots:format:classIndex: */
	assert((YIndex + 1) < (numSlotsMask()));
	newObj = freeStart;
	numBytes = BaseHeaderSize + (((YIndex + 1) < 1
		? 8 /* allocationUnit */
		: (YIndex + 1) * BytesPerOop));
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	if ((freeStart + numBytes) > scavengeThreshold) {
		if (!needGCFlag) {
			/* begin scheduleScavenge */
			needGCFlag = 1;
			forceInterruptCheck();
		}
		if ((freeStart + numBytes) > (((eden).limit))) {
			error("no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
			pointResult = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) (YIndex + 1))) << (numSlotsFullShift()))) + ((((usqInt)((nonIndexablePointerFormat())) << (formatShift()))))) + ClassPointCompactIndex);
	freeStart += numBytes;
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