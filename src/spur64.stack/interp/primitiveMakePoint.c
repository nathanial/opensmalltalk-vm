/* Extracted from interp.c:19086 (function primitiveMakePoint). */

	/* InterpreterPrimitives>>#primitiveMakePoint */

static void
primitiveMakePoint(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt arg;
    usqInt newObj;
    usqInt numBytes;
    usqInt pt;
    sqInt rcvr;
    char *sp;

	rcvr = longAt(stackPointer + (1 * BytesPerWord));
	arg = longAt(stackPointer);
	if (!(/* isFloatOrInt: */
			(((arg & (tagMask())) != 0)
				? !(((arg & (characterTag())) != 0))
				: ((longAt((void *)(arg))) & (classIndexMask())) == ClassFloatCompactIndex))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}

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
			pt = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) (YIndex + 1))) << (numSlotsFullShift()))) + ((((usqInt)((nonIndexablePointerFormat())) << (formatShift()))))) + ClassPointCompactIndex);
	freeStart += numBytes;
	pt = newObj;
	/* end eeInstantiatePoint */
l1:

	/* No need to check since new object is always new. */

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(pt))
	 && (!(isForwarded(pt))));
	assert(validStorePointerUncheckedArgs(XIndex, pt, rcvr));
	longAtput((void *)((pt + BaseHeaderSize) + ((((usqInt)(XIndex) << (shiftForWord()))))),rcvr);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(pt))
	 && (!(isForwarded(pt))));
	assert(validStorePointerUncheckedArgs(YIndex, pt, arg));
	longAtput((void *)((pt + BaseHeaderSize) + ((((usqInt)(YIndex) << (shiftForWord()))))),arg);

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer + (1 * BytesPerWord)),pt);
	stackPointer = sp;
}