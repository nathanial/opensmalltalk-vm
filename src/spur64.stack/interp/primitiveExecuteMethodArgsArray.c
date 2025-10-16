/* Extracted from interp.c:67994 (function primitiveExecuteMethodArgsArray). */

static void
primitiveExecuteMethodArgsArray(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt argCnt;
    sqInt argumentArray;
    usqInt firstBytecode;
    sqInt header;
    sqInt i;
    usqInt iUsqInt;
    sqInt methodArgument;
    sqInt methodHeader;
    usqInt numArgs;
    usqInt numSlots;
    usqInt numTemps;
    sqInt object;
    sqInt primitiveIndex;
    sqInt rcvr;
    sqInt rcvrSqInt;
    char *sp;

	methodArgument = longAt(GIV(stackPointer));
	argumentArray = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (!((/* isOopCompiledMethod: */
			((!(methodArgument & (tagMask()))))
		 && (((byteAt((void *)(methodArgument + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))
		 && (/* isArray: */
			((!(argumentArray & (tagMask()))))
		 && (((byteAt((void *)(argumentArray + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat()))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}

	/* begin argumentCountOf: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(methodArgument));
	header = longAt((void *)((methodArgument + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	argCnt = (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;
	if (!(argCnt == ((/* begin numSlotsOf: */
			assert((classIndexOf(argumentArray)) > (isForwardedObjectClassIndexPun())),
		(((numSlots = byteAt((void *)(argumentArray + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(argumentArray - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadNumArgs;
		return;
	}
	if (GIV(argumentCount) > 2) {
		rcvr = longAt(GIV(stackPointer) + (2 * BytesPerWord));
		if ((GIV(argumentCount) > 4)
		 || (/* isOopForwarded: */
			((!(rcvr & (tagMask()))))
		 && ((!((longAt((void *)(rcvr))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrUnsupported;
			return;
		}

		/* stackValue:put: */
		longAtput(GIV(stackPointer) + (GIV(argumentCount) * BytesPerWord),rcvr);
	}

	/* begin pop: */
	GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
	for (i = 0; i < argCnt; i += 1) {
		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),longAt((void *)((argumentArray + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord())))))));
		GIV(stackPointer) = sp;
	}
	GIV(newMethod) = methodArgument;

	/* begin primitiveIndexOf: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(GIV(newMethod)));
	methodHeader = longAt((void *)((GIV(newMethod) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
		firstBytecode = (GIV(newMethod) + ((LiteralStart + (((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) * BytesPerOop)) + BaseHeaderSize;
		primitiveIndex = (byteAt((void *)(firstBytecode + 1))) + ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
	}
	else {
		primitiveIndex = 0;
	}

	/* begin functionPointerFor:inClass: */
	primitiveFunctionPointer = ((void (*)(void)) (((((usqInt)primitiveIndex)) > MaxPrimitiveIndex
		? 0
		: primitiveTable[primitiveIndex])));
	GIV(argumentCount) = argCnt;

	/* begin executeNewMethod */
	if (primitiveFunctionPointer) {
		if ((((usqIntptr_t) primitiveFunctionPointer)) <= MaxQuickPrimitiveIndex) {
			externalQuickPrimitiveResponse();
			goto l1;
		}
		if (slowPrimitiveResponse()) {
			goto l1;
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
	rcvrSqInt = longAt(GIV(stackPointer) + (numArgs * BytesPerWord));
	assert(!(isOopForwarded(rcvrSqInt)));

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
	longAtput((sp = GIV(stackPointer) - BytesPerWord),rcvrSqInt);
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
l1:

	/* Recursive xeq affects primErrorCode */

	/* begin initPrimCall */
	GIV(primFailCode) = 0;
}