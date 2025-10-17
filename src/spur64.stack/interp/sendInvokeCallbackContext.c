/* Extracted from interp.c:62798 (function sendInvokeCallbackContext). */

/*	Send the calllback message to Alien class with the supplied arg(s). Use
	either the 1 arg
	invokeCallbackContext: or the 4 arg invokeCallback:stack:registers:jmpbuf:
	message, depending on what selector is installed in the
	specialObjectsArray. Note that if invoking the
	legacy invokeCallback:stack:registers:jmpbuf: we pass the
	vmCallbackContext as the jmpbuf
	argument (see reestablishContextPriorToCallback:). The arguments are raw C
	addresses and
	are converted to integer objects on the way. sendInvokeCallbackContext: &
	returnAs:ThroughCallback:Context: along with ownVM: and disownVM: conspire
	to save and
	restore newMethod, argumentCount and primitiveFunctionPointer around a
	callback. The VM depends on argumentCount being correct to cut-back the
	correct number of
	arguments on primitive return. Since this is an implicit send we need to
	log it explicitly.
	The return side is done via a primitive so that gets logged normally. */

	/* StackInterpreter>>#sendInvokeCallbackContext: */

sqInt
sendInvokeCallbackContext(VMCallbackContext *vmCallbackContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classTag;
    usqInt i;
    sqInt methodHeader;
    usqInt numArgs;
    usqInt numTemps;
    sqInt object;
    sqInt rcvr;
    char *sp;


	/* begin recordTrace:thing:source: */
	if (TraceLog) {
		GIV(traceLog)[GIV(traceLogIndex)] = TraceVMCallback;
		GIV(traceLog)[GIV(traceLogIndex) + 1] = (longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SelectorInvokeCallback) << (shiftForWord())))))));
		GIV(traceLog)[GIV(traceLogIndex) + 2] = 0;
		GIV(traceLogIndex) = (GIV(traceLogIndex) + 3) % TraceBufferSize;
	}

	/* begin fetchClassTagOfNonImm: */
	classTag = (longAt((void *)(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassAlien) << (shiftForWord()))))))))) & (classIndexMask());
	GIV(messageSelector) = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SelectorInvokeCallback) << (shiftForWord()))))));
	if (!(lookupInMethodCacheSelclassTag(GIV(messageSelector), classTag))) {
		if (lookupOrdinaryNoMNUEtcInClass(classForClassTag(classTag))) {
			return 0;
		}
	}
	assert(((debugCallbackInvokes += 1)) > 0);

	/* self assert: debugCallbackInvokes < 3802. */

	/* begin saveCStackStateForCallbackContext: */
	memcpy(((void *)((vmCallbackContext->savedReenterInterpreter))), GIV(reenterInterpreter), sizeof(jmp_buf));
	longAtput((sp = GIV(stackPointer) - BytesPerWord),longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassAlien) << (shiftForWord())))))));
	GIV(stackPointer) = sp;
	if ((argumentCountOf(GIV(newMethod))) == 4) {
		object = positive64BitIntegerFor(((usqInt)((vmCallbackContext->thunkp))));

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
		GIV(stackPointer) = sp;
		object = positive64BitIntegerFor(((usqInt)((vmCallbackContext->stackp))));

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
		GIV(stackPointer) = sp;
		object = positive64BitIntegerFor(((usqInt)((vmCallbackContext->intregargsp))));

		/* begin push: */
		longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
		GIV(stackPointer) = sp;
	}
	object = positive64BitIntegerFor(((usqInt)vmCallbackContext));

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
	GIV(stackPointer) = sp;

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
	for (i = (numArgs + 1); i <= numTemps; i += 1) {
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

	/* begin checkForStackOverflow */
	/* begin externalWriteBackHeadFramePointers */
	assert((GIV(framePointer) - GIV(stackPointer)) < (LargeContextSlots * BytesPerOop));
	assert(GIV(stackPage) == (GIV(mostRecentlyUsedPage)));
	assert(!((isFree(GIV(stackPage)))));

	/* begin setHeadFP:andSP:inPage: */
	assert(GIV(stackPointer) < GIV(framePointer));
	assert((GIV(stackPointer) < ((GIV(stackPage)->baseAddress)))
	 && (GIV(stackPointer) > (((GIV(stackPage)->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
	assert((GIV(framePointer) < ((GIV(stackPage)->baseAddress)))
	 && (GIV(framePointer) > (((GIV(stackPage)->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
	(GIV(stackPage)->headFP = GIV(framePointer));
	(GIV(stackPage)->headSP = GIV(stackPointer));
	assert(pageListIsWellFormed());
	if (GIV(stackPointer) < ((GIV(stackPage)->realStackLimit))) {
		handleStackOverflow();
	}
	assert((frameReceiver(GIV(framePointer))) == (splObj(ClassAlien)));

	/* begin enterSmalltalkExecutiveFromCallback */
	enterSmalltalkExecutive();

	/* not reached */
	return 1;
}