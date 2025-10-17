/* Extracted from interp.c:13169 (function primitiveBytecodeSetsAvailable). */

/*	Answer the encoder names for the supported bytecode sets. */

	/* InterpreterPrimitives>>#primitiveBytecodeSetsAvailable */

EXPORT(sqInt)
primitiveBytecodeSetsAvailable(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt encoderNames;
    usqInt newObj;
    usqInt numBytes;
    char *sp;
    sqInt valuePointer;

	if (GIV(argumentCount) > 0) {
		return (GIV(primFailCode) = PrimErrBadNumArgs);
	}

	/* begin eeInstantiateSmallClassIndex:format:numSlots: */
	assert((knownClassAtIndex(ClassArrayCompactIndex)) != GIV(nilObj));
	assert((arrayFormat()) == (instSpecOfClass(knownClassAtIndex(ClassArrayCompactIndex))));

	/* begin allocateSmallNewSpaceSlots:format:classIndex: */
	newObj = GIV(freeStart);
	numBytes = BaseHeaderSize + (2 * BytesPerOop);
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
			encoderNames = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) 2)) << (numSlotsFullShift()))) + ((((usqInt)((arrayFormat())) << (formatShift()))))) + ClassArrayCompactIndex);
	GIV(freeStart) += numBytes;
	encoderNames = newObj;
	/* end eeInstantiateSmallClassIndex:format:numSlots: */
l1:
	valuePointer = stringForCString("EncoderForV3PlusClosures");

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(encoderNames))
	 && (!(isForwarded(encoderNames))));
	assert(validStorePointerUncheckedArgs(0, encoderNames, valuePointer));
	longAtput((void *)((encoderNames + BaseHeaderSize) + (0U << (shiftForWord()))),valuePointer);
	valuePointer = stringForCString("EncoderForSistaV1");

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(encoderNames))
	 && (!(isForwarded(encoderNames))));
	assert(validStorePointerUncheckedArgs(1, encoderNames, valuePointer));
	longAtput((void *)((encoderNames + BaseHeaderSize) + (1U << (shiftForWord()))),valuePointer);

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),encoderNames);
	GIV(stackPointer) = sp;
	return 0;
}