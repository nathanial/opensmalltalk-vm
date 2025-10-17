/* Extracted from interp.c:69216 (function primitivePerform). */

	/* StackInterpreterPrimitives>>#primitivePerform */

static void
primitivePerform(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    usqInt iUsqInt;
    sqInt lookupClassTag;
    sqInt methodHeader;
    sqInt newReceiver;
    usqInt numArgs;
    usqInt numTemps;
    sqInt object;
    usqInt performMethod;
    sqInt rcvr;
    char *sp;
    sqInt tagBits;

	performMethod = GIV(newMethod);
	GIV(messageSelector) = longAt(GIV(stackPointer) + ((GIV(argumentCount) - 1) * BytesPerWord));
	newReceiver = longAt(GIV(stackPointer) + (GIV(argumentCount) * BytesPerWord));

	/* NOTE: the following lookup may fail and be converted to #doesNotUnderstand:,
	   so we must adjust argumentCount and slide args now, so that will work.
	   Slide arguments down over selector */
	GIV(argumentCount) -= 1;
	for (i = GIV(argumentCount); i >= 1; i += -1) {
		longAtput(GIV(stackPointer) + (i * BytesPerWord),longAt(GIV(stackPointer) + ((i - 1) * BytesPerWord)));
	}

	/* begin pop: */
	GIV(stackPointer) += 1 * BytesPerWord;
	lookupClassTag = /* fetchClassTagOf: */
			((tagBits = newReceiver & (tagMask()))
				? tagBits
				: (longAt((void *)(newReceiver))) & (classIndexMask()));

	/* begin sendBreakpoint:receiver: */
	sendBreakpointclassTag(firstFixedFieldOfMaybeImmediate(GIV(messageSelector)), lengthOfMaybeImmediate(GIV(messageSelector)), /* fetchClassTagOf: */
		((tagBits = newReceiver & (tagMask()))
			? tagBits
			: (longAt((void *)(newReceiver))) & (classIndexMask())));
	if (
#  if SEND_PRINTING
		GIV(printSends)
#  else
		0
#  endif
		) {
		printActivationNameForSelectorstartClass(GIV(messageSelector), classForClassTag(lookupClassTag));
		cr();
	}
	findNewMethodInClassTag(lookupClassTag);

	/* Only test CompiledMethods for argument count - other objects will have to take their chances */
	if (!((/* isOopCompiledMethod: */
			((!(GIV(newMethod) & (tagMask()))))
		 && (((byteAt((void *)(GIV(newMethod) + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))
		 && ((argumentCountOf(GIV(newMethod))) == GIV(argumentCount)))) {
		/* begin unPop: */
		GIV(stackPointer) -= 1 * BytesPerWord;
		for (i = 1; i <= GIV(argumentCount); i += 1) {
			longAtput(GIV(stackPointer) + ((i - 1) * BytesPerWord),longAt(GIV(stackPointer) + (i * BytesPerWord)));
		}
		longAtput(GIV(stackPointer) + (GIV(argumentCount) * BytesPerWord),GIV(messageSelector));
		GIV(argumentCount) += 1;
		GIV(newMethod) = performMethod;

		/* Must reset primitiveFunctionPointer for checkForAndFollowForwardedPrimitiveState */
		primitiveFunctionPointer = primitivePerform;

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadNumArgs;
		return;
	}

	/* Slide the args back up (sigh) and re-insert the selector. */

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
l1:

	/* Recursive xeq affects primErrorCode */

	/* begin initPrimCall */
	GIV(primFailCode) = 0;
}