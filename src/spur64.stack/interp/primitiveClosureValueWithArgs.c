/* Extracted from interp.c:13950 (function primitiveClosureValueWithArgs). */

	/* InterpreterPrimitives>>#primitiveClosureValueWithArgs */

static void
primitiveClosureValueWithArgs(void)
{
    sqInt argumentArray;
    usqInt arraySize;
    sqInt blockClosure;
    sqInt closureIP;
    sqInt closureMethod;
    sqInt i;
    sqInt index;
    sqInt methodHeader;
    sqInt numArgs;
    sqInt numCopied;
    usqInt numSlots;
    sqInt object;
    sqInt objOop;
    sqInt oop;
    sqInt outerContext;
    char *sp;
    sqInt top;

	argumentArray = longAt(stackPointer);
	if (!(/* isArray: */
			((!(argumentArray & (tagMask()))))
		 && (((byteAt((void *)(argumentArray + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return;
	}

	/* Check for enough space in thisContext to push all args */

	/* begin numSlotsOf: */
	assert((classIndexOf(argumentArray)) > (isForwardedObjectClassIndexPun()));
	arraySize = (((numSlots = byteAt((void *)(argumentArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(argumentArray - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlots);
	if (!(arraySize <= (LargeContextSlots - CtxtTempFrameStart))) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return;
	}
	blockClosure = longAt(stackPointer + (argumentCount * BytesPerWord));

	/* begin argumentCountOfClosure: */
	/* begin quickFetchInteger:ofObject: */
	oop = longAt((void *)((blockClosure + BaseHeaderSize) + ((((usqInt)(ClosureNumArgsIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	numArgs = (oop >> 3);
	if (!(arraySize == numArgs)) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return;
	}

	/* Somewhat paranoiac checks we need while debugging that we may be able to discard
	   in a robust system. */
	outerContext = longAt((void *)((blockClosure + BaseHeaderSize) + ((((usqInt)(ClosureOuterContextIndex) << (shiftForWord()))))));
	if (!(/* isContext: */
			((!(outerContext & (tagMask()))))
		 && (((longAt((void *)(outerContext))) & (classIndexMask())) == ClassMethodContextCompactIndex))) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return;
	}
	closureMethod = longAt((void *)((outerContext + BaseHeaderSize) + ((((usqInt)(MethodIndex) << (shiftForWord()))))));

	/* Check if the closure's method is actually a CompiledMethod. */
	if (!(/* isOopCompiledMethod: */
			((!(closureMethod & (tagMask()))))
		 && (((byteAt((void *)(closureMethod + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
		/* begin primitiveFail */
		if (!primFailCode) {
			primFailCode = 1;
		}
		return;
	}

	/* begin popStack */
	top = longAt(stackPointer);
	stackPointer += BytesPerWord;

	/* Copy the arguments to the stack, and activate */
	index = 1;
	while (index <= numArgs) {
		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),longAt((void *)((argumentArray + BaseHeaderSize) + ((((usqInt)((index - 1)) << (shiftForWord())))))));
		stackPointer = sp;
		index += 1;
	}

	/* begin activateNewClosure:outer:method:numArgs:mayContextSwitch: */
	assert(isContext(outerContext));
	assert(isVanillaBlockClosure(blockClosure));
	numCopied = ((/* begin numSlotsOf: */
	assert((classIndexOf(blockClosure)) > (isForwardedObjectClassIndexPun())),
(((numSlots = byteAt((void *)(blockClosure + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
			? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(blockClosure - BaseHeaderSize)))) << 8)))))) >> 8
			: numSlots))) - ClosureFirstCopiedValueIndex;
	assert(closureMethod == (fetchPointerofObject(MethodIndex, outerContext)));
	assert(isOopCompiledMethod(closureMethod));

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),instructionPointer);
	stackPointer = sp;

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),((usqInt)framePointer));
	stackPointer = sp;
	framePointer = stackPointer;

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),closureMethod);
	stackPointer = sp;
	object = /* encodeFrameFieldHasContext:isBlock:numArgs: */
			(VMBIGENDIAN
				? ((1 + ((((usqInt)(numArgs) << ((BytesPerWord * 8) - 8)))))) + (1ULL << ((BytesPerWord * 8) - 24))
				: ((1 + ((((usqInt)(numArgs) << 8))))) + (0x1000000));

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),object);
	stackPointer = sp;

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),nilObj);
	stackPointer = sp;

	/* begin followField:ofObject: */
	objOop = longAt((void *)((outerContext + BaseHeaderSize) + ((((usqInt)(ReceiverIndex) << (shiftForWord()))))));
	if (/* isOopForwarded: */
		((!(objOop & (tagMask()))))
	 && ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		objOop = fixFollowedFieldofObjectwithInitialValue(ReceiverIndex, outerContext, objOop);
	}
	object = objOop;

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),object);
	stackPointer = sp;

	/* Copy the copied values... */
	for (i = 0; i < numCopied; i += 1) {
		/* begin push: */
		longAtput((sp = stackPointer - BytesPerWord),longAt((void *)((blockClosure + BaseHeaderSize) + ((((usqInt)((i + ClosureFirstCopiedValueIndex)) << (shiftForWord())))))));
		stackPointer = sp;
	}
	assert(frameIsBlockActivation(framePointer));
	assert(!(frameHasContext(framePointer)));

	/* The initial instructions in the block nil-out remaining temps.
	   the instruction pointer is a pointer variable equal to
	   method oop + ip + BaseHeaderSize
	   -1 for 0-based addressing of fetchByte
	   -1 because it gets incremented BEFORE fetching currentByte */

	/* begin quickFetchInteger:ofObject: */
	oop = longAt((void *)((blockClosure + BaseHeaderSize) + ((((usqInt)(ClosureStartPCIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	closureIP = (oop >> 3);
	instructionPointer = ((closureMethod + closureIP) + BaseHeaderSize) - 2;

	/* begin setMethod: */
	method = closureMethod;
	assert(isOopCompiledMethod(method));

	/* begin methodUsesAlternateBytecodeSet: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(method));
	methodHeader = longAt((void *)((method + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	if ((((sqLong) methodHeader)) < 0) {
		bytecodeSetSelector = 0x100;
	}
	else {
		bytecodeSetSelector = 0;
	}

	/* Now check for stack overflow or an event (interrupt, must scavenge, etc) */
	if (stackPointer < stackLimit) {
		handleStackOverflowOrEventAllowContextSwitch(1);
	}
}