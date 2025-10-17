/* Extracted from interp.c:66342 (function primitiveClosureCopyWithCopiedValues). */

	/* StackInterpreterPrimitives>>#primitiveClosureCopyWithCopiedValues */

static void
primitiveClosureCopyWithCopiedValues(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt context;
    sqInt copiedValues;
    sqInt fieldIndex;
    sqInt i;
    sqInt initialIP;
    sqInt integerPointer;
    usqInt newClosure;
    usqInt newObj;
    sqInt numArgs;
    usqInt numBytes;
    usqInt numCopied;
    usqInt numSlots;
    sqInt numSlotsSqInt;
    char *sp;
    sqInt valuePointer;

	/* begin stackIntegerValue: */
	integerPointer = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if ((((integerPointer) & 7) == 1)) {
		numArgs = (integerPointer >> 3);
	}
	else {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		numArgs = 0;
	}
	if (GIV(primFailCode)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}
	context = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	initialIP = (GIV(instructionPointer) + 2) - (GIV(method) + BaseHeaderSize);
	copiedValues = longAt(GIV(stackPointer));

	/* begin closureIn:numArgs:instructionPointer:copiedValues: */
	/* begin numSlotsOf: */
	assert((classIndexOf(copiedValues)) > (isForwardedObjectClassIndexPun()));
	numCopied = (((numSlots = byteAt((void *)(copiedValues + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(copiedValues - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	numSlotsSqInt = ClosureFirstCopiedValueIndex + numCopied;

	/* begin eeInstantiateSmallClassIndex:format:numSlots: */
	assert((numSlotsSqInt >= 0)
	 && ((knownClassAtIndex(ClassBlockClosureCompactIndex)) != GIV(nilObj)));
	assert((indexablePointersFormat()) == (instSpecOfClass(knownClassAtIndex(ClassBlockClosureCompactIndex))));

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
			newClosure = 0;
			goto l1;
		}
	}
	long64Atput((void *)(newObj),((((((usqLong) numSlotsSqInt)) << (numSlotsFullShift()))) + ((((usqInt)((indexablePointersFormat())) << (formatShift()))))) + ClassBlockClosureCompactIndex);
	GIV(freeStart) += numBytes;
	newClosure = newObj;
	/* end eeInstantiateSmallClassIndex:format:numSlots: */
l1:

	/* Assume: have just allocated a new closure; it must be young. Thus, can use unchecked stores. */

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(newClosure))
	 && (!(isForwarded(newClosure))));
	assert(validStorePointerUncheckedArgs(ClosureOuterContextIndex, newClosure, context));
	longAtput((void *)((newClosure + BaseHeaderSize) + ((((usqInt)(ClosureOuterContextIndex) << (shiftForWord()))))),context);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(newClosure))
	 && (!(isForwarded(newClosure))));
	assert(validStorePointerUncheckedArgs(ClosureStartPCIndex, newClosure, (((usqInt)initialIP << 3) | 1)));
	longAtput((void *)((newClosure + BaseHeaderSize) + ((((usqInt)(ClosureStartPCIndex) << (shiftForWord()))))),(((usqInt)initialIP << 3) | 1));

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(newClosure))
	 && (!(isForwarded(newClosure))));
	assert(validStorePointerUncheckedArgs(ClosureNumArgsIndex, newClosure, (((usqInt)numArgs << 3) | 1)));
	longAtput((void *)((newClosure + BaseHeaderSize) + ((((usqInt)(ClosureNumArgsIndex) << (shiftForWord()))))),(((usqInt)numArgs << 3) | 1));
	for (i = 0; i < numCopied; i += 1) {
		fieldIndex = i + ClosureFirstCopiedValueIndex;
		valuePointer = longAt((void *)((copiedValues + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(newClosure))
		 && (!(isForwarded(newClosure))));
		assert(validStorePointerUncheckedArgs(fieldIndex, newClosure, valuePointer));
		longAtput((void *)((newClosure + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),valuePointer);
	}

	/* greater by 1 due to preIncrement of localIP */

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (2 * BytesPerWord)),newClosure);
	GIV(stackPointer) = sp;
}