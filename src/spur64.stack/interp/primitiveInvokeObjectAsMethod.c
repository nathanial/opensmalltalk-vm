/* Extracted from interp.c:18469 (function primitiveInvokeObjectAsMethod). */

/*	Primitive. 'Invoke' an object like a function, sending the special message
	run: originalSelector with: arguments in: aReceiver.
	 */

	/* InterpreterPrimitives>>#primitiveInvokeObjectAsMethod */

static void
primitiveInvokeObjectAsMethod(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    usqInt iUsqInt;
    sqInt lookupClassTag;
    sqInt methodHeader;
    usqInt newObj;
    usqInt numArgs;
    usqInt numBytes;
    usqInt numTemps;
    sqInt object;
    sqInt rcvr;
    sqInt runArgs;
    sqInt runReceiver;
    char *sp;
    sqInt tagBits;
    sqInt top;
    sqInt valuePointer;

	/* begin eeInstantiateClassIndex:format:numSlots: */
	assert((GIV(argumentCount) >= 0)
	 && ((knownClassAtIndex(ClassArrayCompactIndex)) != GIV(nilObj)));
	assert((arrayFormat()) == (instSpecOfClass(knownClassAtIndex(ClassArrayCompactIndex))));

	/* begin allocateNewSpaceSlots:format:classIndex: */
	if (GIV(argumentCount) >= (numSlotsMask())) {
		if (GIV(argumentCount) > 0xFFFFFFFFU) {
			runArgs = ((usqInt) null);
			goto l1;
		}
		newObj = GIV(freeStart) + BaseHeaderSize;
		numBytes = (BaseHeaderSize + BaseHeaderSize) + (GIV(argumentCount) * BytesPerOop);
	}
	else {
		newObj = GIV(freeStart);
		numBytes = BaseHeaderSize + ((GIV(argumentCount) < 1
		? 8 /* allocationUnit */
		: GIV(argumentCount) * BytesPerOop));
	}
	if ((GIV(freeStart) + numBytes) > GIV(scavengeThreshold)) {
		if (!GIV(needGCFlag)) {
			/* begin scheduleScavenge */
			GIV(needGCFlag) = 1;
			forceInterruptCheck();
		}
		if ((GIV(freeStart) + numBytes) > (((GIV(eden)).limit))) {
			error("no room in eden for allocateNewSpaceSlots:format:classIndex:");
			runArgs = 0;
			goto l1;
		}
	}
	if (GIV(argumentCount) >= (numSlotsMask())) {
		longAtput((void *)(GIV(freeStart)),GIV(argumentCount));
		longAtput((void *)(GIV(freeStart) + 4),((sqInt)((usqInt)((numSlotsMask())) << (numSlotsHalfShift()))));
		long64Atput((void *)(newObj),((((((usqLong) (numSlotsMask()))) << (numSlotsFullShift()))) + ((((usqInt)((arrayFormat())) << (formatShift()))))) + ClassArrayCompactIndex);
	}
	else {
		long64Atput((void *)(newObj),((((((usqLong) GIV(argumentCount))) << (numSlotsFullShift()))) + ((((usqInt)((arrayFormat())) << (formatShift()))))) + ClassArrayCompactIndex);
	}

	/* for header parsing we put a saturated slot count in the prepended overflow size word */
	assert((numBytes % (allocationUnit())) == 0);
	assert((newObj % (allocationUnit())) == 0);
	GIV(freeStart) += numBytes;
	runArgs = newObj;
	/* end eeInstantiateClassIndex:format:numSlots: */
l1:
	for (i = (GIV(argumentCount) - 1); i >= 0; i += -1) {
		/* begin popStack */
		top = longAt(GIV(stackPointer));
		GIV(stackPointer) += BytesPerWord;
		valuePointer = top;

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(runArgs))
		 && (!(isForwarded(runArgs))));
		assert(validStorePointerUncheckedArgs(i, runArgs, valuePointer));
		longAtput((void *)((runArgs + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))),valuePointer);
	}

	/* begin popStack */
	runReceiver = longAt(GIV(stackPointer));
	GIV(stackPointer) += BytesPerWord;

	/* setup send of newMethod run: originalSelector with: runArgs in: runReceiver */

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(newMethod));
	GIV(stackPointer) = sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(messageSelector));
	GIV(stackPointer) = sp;

	/* original selector */

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),runArgs);
	GIV(stackPointer) = sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),runReceiver);
	GIV(stackPointer) = sp;

	/* stack is clean here */
	GIV(messageSelector) = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SelectorRunWithIn) << (shiftForWord()))))));
	GIV(argumentCount) = 3;
	lookupClassTag = /* fetchClassTagOf: */
			((tagBits = GIV(newMethod) & (tagMask()))
				? tagBits
				: (longAt((void *)(GIV(newMethod)))) & (classIndexMask()));
	findNewMethodInClassTag(lookupClassTag);

	/* begin executeNewMethod */
	if (primitiveFunctionPointer) {
		if ((((usqIntptr_t) primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex) {
			externalQuickPrimitiveResponse();
			goto l2;
		}
		if (slowPrimitiveResponse()) {
			goto l2;
		}
	}

	/* if not primitive, or primitive failed, activate the method */

	/* begin activateNewMethod */
	/* begin justActivateNewMethod: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(GIV(newMethod)));
	methodHeader = longAt((void *)((GIV(newMethod) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	numTemps = (((usqInt)(methodHeader)) >> MethodHeaderTempCountShift) & 0x3F;
	numArgs = (((usqInt)(methodHeader)) >> MethodHeaderArgCountShift) & 15;

	/* could new rcvr be set at point of send? */
	rcvr = longAt(GIV(stackPointer) + (numArgs * BytesPerWord));
	assert(!(isOopForwarded(rcvr)));

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(instructionPointer));
	GIV(stackPointer) = sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),((usqInt)GIV(framePointer)));
	GIV(stackPointer) = sp;
	GIV(framePointer) = GIV(stackPointer);

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(newMethod));
	GIV(stackPointer) = sp;

	/* begin setMethod:methodHeader: */
	GIV(method) = GIV(newMethod);
	assert(isOopCompiledMethod(GIV(method)));
	assert((methodHeaderOf(GIV(method))) == methodHeader);
	GIV(bytecodeSetSelector) = ((((sqLong) methodHeader)) < 0
				? 0x100
				: 0);
	object = /* encodeFrameFieldHasContext:isBlock:numArgs: */
			(VMBIGENDIAN
				? ((1 + ((numArgs << ((BytesPerWord * 8) - 8)))))
				: ((1 + ((numArgs << 8)))));

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
	GIV(stackPointer) = sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(nilObj));
	GIV(stackPointer) = sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),rcvr);
	GIV(stackPointer) = sp;

	/* clear remaining temps to nil */
	for (iUsqInt = (numArgs + 1); iUsqInt <= numTemps; iUsqInt += 1) {
		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(nilObj));
		GIV(stackPointer) = sp;
	}
	GIV(instructionPointer) = (((((usqInt)(pointerForOop(GIV(newMethod))))) + ((LiteralStart + ((/* begin literalCountOfMethodHeader: */
	assert((((methodHeader) & 7) == 1)),
/* literalCountOfAlternateHeader: */
	((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask))) * BytesPerOop)) + BaseHeaderSize) - 1;
	if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
		GIV(instructionPointer) += 3 /* sizeOfCallPrimitiveBytecode: */;
		if (GIV(primFailCode)) {
			reapAndResetErrorCodeToheader(GIV(stackPointer), methodHeader);
		}
	}

	/* Skip the CallPrimitive bytecode, if it's there, and store the error code if the method starts
	   with a long store temp.  Strictly no need to skip the store because it's effectively a noop. */

	/* Now check for stack overflow or an event (interrupt, must scavenge, etc). */
	if (GIV(stackPointer) < GIV(stackLimit)) {
		handleStackOverflowOrEventAllowContextSwitch(canContextSwitchIfActivatingheader(GIV(newMethod), methodHeader));
	}
	/* end executeNewMethod */
l2:

	/* begin initPrimCall */
	GIV(primFailCode) = 0;
}