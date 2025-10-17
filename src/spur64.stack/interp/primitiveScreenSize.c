/* Extracted from interp.c:21839 (function primitiveScreenSize). */

/*	Answer a point indicating the current size of the Smalltalk window.
	Currently there is a limit of 65535 in each direction because the
	point is encoded into a single 32bit value in the image header.
	This might well become a problem one day */

	/* InterpreterPrimitives>>#primitiveScreenSize */

static void
primitiveScreenSize(void)
{
    usqInt newObj;
    usqInt numBytes;
    sqInt oop;
    usqInt pointResult;
    sqInt pointWord;
    char *sp;
    sqInt xValue;
    sqInt yValue;

	pointWord = ioScreenSize();
	xValue = (((usqInt)(pointWord)) >> 16) & 0xFFFF;
	yValue = pointWord & 0xFFFF;

	/* begin makePointwithxValue:yValue: */
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
	oop = pointResult;

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer),oop);
	stackPointer = sp;
}