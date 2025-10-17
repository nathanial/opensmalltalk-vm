/* Extracted from interp.c:13800 (function primitiveClosureValueNoContextSwitch). */

/*	An exact clone of primitiveClosureValue except that this version will not
	check for interrupts on stack overflow. It may invoke the garbage
	collector but will not switch processes. See
	checkForInterruptsMayContextSwitch:  */

	/* InterpreterPrimitives>>#primitiveClosureValueNoContextSwitch */

void
primitiveClosureValueNoContextSwitch(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt blockClosure;
    sqInt closureIP;
    sqInt closureMethod;
    sqInt i;
    sqInt methodHeader;
    sqInt numArgs;
    sqInt numCopied;
    usqInt numSlots;
    sqInt object;
    sqInt objOop;
    sqInt oop;
    sqInt outerContext;
    char *sp;

	blockClosure = longAt(GIV(stackPointer) + (GIV(argumentCount) * BytesPerWord));

	/* begin argumentCountOfClosure: */
	/* begin quickFetchInteger:ofObject: */
	oop = longAt((void *)((blockClosure + BaseHeaderSize) + ((((usqInt)(ClosureNumArgsIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	numArgs = (oop >> 3);
	if (!(GIV(argumentCount) == numArgs)) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
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
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}
	closureMethod = longAt((void *)((outerContext + BaseHeaderSize) + ((((usqInt)(MethodIndex) << (shiftForWord()))))));

	/* Check if the closure's method is actually a CompiledMethod. */
	if (!(/* isOopCompiledMethod: */
			((!(closureMethod & (tagMask()))))
		 && (((byteAt((void *)(closureMethod + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
		/* begin primitiveFail */
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
		return;
	}
	GIV(deferProfileCheckForVNCS) = 1;

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
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(instructionPointer));
	GIV(stackPointer) = sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),((usqInt)GIV(framePointer)));
	GIV(stackPointer) = sp;
	GIV(framePointer) = GIV(stackPointer);

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),closureMethod);
	GIV(stackPointer) = sp;
	object = /* encodeFrameFieldHasContext:isBlock:numArgs: */
			(VMBIGENDIAN
				? ((1 + ((((usqInt)(numArgs) << ((BytesPerWord * 8) - 8)))))) + (1ULL << ((BytesPerWord * 8) - 24))
				: ((1 + ((((usqInt)(numArgs) << 8))))) + (0x1000000));

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
	GIV(stackPointer) = sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(nilObj));
	GIV(stackPointer) = sp;

	/* begin followField:ofObject: */
	objOop = longAt((void *)((outerContext + BaseHeaderSize) + ((((usqInt)(ReceiverIndex) << (shiftForWord()))))));
	if (/* isOopForwarded: */
		((!(objOop & (tagMask()))))
	 && ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		objOop = fixFollowedFieldofObjectwithInitialValue(ReceiverIndex, outerContext, objOop);
	}
	object = objOop;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
	GIV(stackPointer) = sp;

	/* Copy the copied values... */
	for (i = 0; i < numCopied; i += 1) {
		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),longAt((void *)((blockClosure + BaseHeaderSize) + ((((usqInt)((i + ClosureFirstCopiedValueIndex)) << (shiftForWord())))))));
		GIV(stackPointer) = sp;
	}
	assert(frameIsBlockActivation(GIV(framePointer)));
	assert(!(frameHasContext(GIV(framePointer))));

	/* The initial instructions in the block nil-out remaining temps.
	   the instruction pointer is a pointer variable equal to
	   method oop + ip + BaseHeaderSize
	   -1 for 0-based addressing of fetchByte
	   -1 because it gets incremented BEFORE fetching currentByte */

	/* begin quickFetchInteger:ofObject: */
	oop = longAt((void *)((blockClosure + BaseHeaderSize) + ((((usqInt)(ClosureStartPCIndex) << (shiftForWord()))))));
	assert((((oop) & 7) == 1));
	closureIP = (oop >> 3);
	GIV(instructionPointer) = ((closureMethod + closureIP) + BaseHeaderSize) - 2;

	/* begin setMethod: */
	GIV(method) = closureMethod;
	assert(isOopCompiledMethod(GIV(method)));

	/* begin methodUsesAlternateBytecodeSet: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(GIV(method)));
	methodHeader = longAt((void *)((GIV(method) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	if ((((sqLong) methodHeader)) < 0) {
		GIV(bytecodeSetSelector) = 0x100;
	}
	else {
		GIV(bytecodeSetSelector) = 0;
	}

	/* Now check for stack overflow or an event (interrupt, must scavenge, etc) */
	if (GIV(stackPointer) < GIV(stackLimit)) {
		handleStackOverflowOrEventAllowContextSwitch(0);
	}
}