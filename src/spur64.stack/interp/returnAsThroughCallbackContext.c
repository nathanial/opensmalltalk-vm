/* Extracted from interp.c:62436 (function returnAsThroughCallbackContext). */

sqInt
returnAsThroughCallbackContext(sqInt returnTypeOop, VMCallbackContext *vmCallbackContext, sqInt callbackMethodContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt calloutMethodContext;
    sqInt methodHeader;
    sqInt senderOop;
    char *sp;
    char *theFP;
    StackPage *thePage;
    sqInt top;

	assert(GIV(primFailCode) == 0);
	assert((((returnTypeOop) & 7) == 1));
	assert(!(isImmediate(((sqInt)vmCallbackContext))));
	assert((addressCouldBeObj(callbackMethodContext))
	 && (isContext(callbackMethodContext)));
	assert(((debugCallbackPath = 0)) == 0);
	if (!(((((returnTypeOop) & 7) == 1))
		 && (isLiveContext(callbackMethodContext)))) {
		assert(((debugCallbackPath = 1)) == 1);
		return 0;
	}
	calloutMethodContext = externalInstVarofContext(SenderIndex, callbackMethodContext);
	if (!(isLiveContext(calloutMethodContext))) {
		assert(((debugCallbackPath = 2)) == 2);
		return 0;
	}
	assert(((debugCallbackReturns += 1)) > 0);

	/* begin recordTrace:thing:source: */
	if (TraceLog) {
		GIV(traceLog)[GIV(traceLogIndex)] = TraceVMCallbackReturn;
		GIV(traceLog)[GIV(traceLogIndex) + 1] = returnTypeOop;
		GIV(traceLog)[GIV(traceLogIndex) + 2] = 0;
		GIV(traceLogIndex) = (GIV(traceLogIndex) + 3) % TraceBufferSize;
	}

	/* We're about to leave this stack page; must save the current frame's instructionPointer. */

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),GIV(instructionPointer));
	GIV(stackPointer) = sp;

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

	/* Mark callbackMethodContext as dead; the common case is that it is the current frame.
	   We go the extra mile for the debugger. */
	if ((!((longAt((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))))) & (tagMask())))) {
		assert(((debugCallbackPath = debugCallbackPath | 4)) > 0);

		/* begin markContextAsDead: */
		assert(isContext(callbackMethodContext));
		assert((isNonImmediate(callbackMethodContext))
		 && (!(isForwarded(callbackMethodContext))));
		assert(validStorePointerUncheckedArgs(SenderIndex, callbackMethodContext, GIV(nilObj)));
		longAtput((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),GIV(nilObj));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(callbackMethodContext))
		 && (!(isForwarded(callbackMethodContext))));
		assert(validStorePointerUncheckedArgs(InstructionPointerIndex, callbackMethodContext, GIV(nilObj)));
		longAtput((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),GIV(nilObj));
	}
	else {
		assert(((debugCallbackPath = debugCallbackPath | 8)) > 0);

		/* begin frameOfMarriedContext: */
		senderOop = longAt((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
		assert((((senderOop) & 7) == 1));
		theFP = ((char *)(senderOop - (smallIntegerTag())));
		assert((frameReceiver(theFP)) == (splObj(ClassAlien)));
		if (GIV(framePointer) == theFP) {
			assert(((debugCallbackPath = debugCallbackPath | 16)) > 0);
			if (longAt(theFP + FoxSavedFP)) {
				assert(((debugCallbackPath = debugCallbackPath | 32)) > 0);
				GIV(instructionPointer) = longAt(theFP + FoxCallerSavedIP);
				GIV(stackPointer) = (theFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord;
				GIV(framePointer) = ((char *)(longAt(theFP + FoxSavedFP)));

				/* begin setMethod: */
				GIV(method) = longAt(GIV(framePointer) + FoxMethod);
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

				/* begin restoreCStackStateForCallbackContext: */
				memcpy(GIV(reenterInterpreter), ((void *)((vmCallbackContext->savedReenterInterpreter))), sizeof(jmp_buf));
				assertValidExecutionPointersimbarline(GIV(instructionPointer), GIV(framePointer), GIV(stackPointer), 1 /* (isMachineCodeFrame: not) */, __LINE__);

				/* N.B. siglongjmp is defines as _longjmp on non-win32 platforms.
				   This matches the use of _setjmp in ia32abicc.c. */
				_longjmp((vmCallbackContext->trampoline), (returnTypeOop >> 3));
				return 1;
			}

			/* calloutMethodContext is immediately below on the same page.  Make it current. */
			freeStackPage(GIV(stackPage));
		}
		else {
			assert(((debugCallbackPath = debugCallbackPath | 64)) > 0);
			externalDivorceFrameandContext(theFP, callbackMethodContext);

			/* begin markContextAsDead: */
			assert(isContext(callbackMethodContext));
			assert((isNonImmediate(callbackMethodContext))
			 && (!(isForwarded(callbackMethodContext))));
			assert(validStorePointerUncheckedArgs(SenderIndex, callbackMethodContext, GIV(nilObj)));
			longAtput((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),GIV(nilObj));

			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(callbackMethodContext))
			 && (!(isForwarded(callbackMethodContext))));
			assert(validStorePointerUncheckedArgs(InstructionPointerIndex, callbackMethodContext, GIV(nilObj)));
			longAtput((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),GIV(nilObj));
		}
	}

	/* Make the calloutMethodContext the active frame.  The case where calloutMethodContext
	   is immediately below callbackMethodContext on the same page is handled above. */
	if (/* isStillMarriedContext: */
		(((((longAt((void *)((calloutMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
	 && (!(isWidowedContext(calloutMethodContext)))) {
		assert(((debugCallbackPath = debugCallbackPath | 128)) > 0);

		/* begin frameOfMarriedContext: */
		senderOop = longAt((void *)((calloutMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
		assert((((senderOop) & 7) == 1));
		theFP = ((char *)(senderOop - (smallIntegerTag())));

		/* begin stackPageFor: */
		thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));

		/* findSPOf:on: points to the word beneath the instructionPointer, but
		   there is no instructionPointer on the top frame of the current page. */
		assert(thePage != GIV(stackPage));
		GIV(stackPointer) = (((thePage->headFP)) == theFP
					? (thePage->headSP)
					: (findSPOfon(theFP, thePage)) - BytesPerWord);
		GIV(framePointer) = theFP;
		assert(GIV(stackPointer) < GIV(framePointer));
	}
	else {
		assert(((debugCallbackPath = debugCallbackPath | 0x100)) > 0);
		thePage = makeBaseFrameFor(calloutMethodContext);

		/* begin setStackPointersFromPage: */
		GIV(stackPointer) = (thePage->headSP);
		GIV(framePointer) = (thePage->headFP);
	}

	/* begin popStack */
	top = longAt(GIV(stackPointer));
	GIV(stackPointer) += BytesPerWord;
	GIV(instructionPointer) = top;

	/* begin setMethod: */
	GIV(method) = longAt((void *)((calloutMethodContext + BaseHeaderSize) + ((((usqInt)(MethodIndex) << (shiftForWord()))))));
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

	/* begin setStackPageAndLimit: */
	assert(thePage);
	GIV(stackPage) = thePage;
	if (GIV(stackLimit) != (((char *) (((usqInt) -1))))) {
		GIV(stackLimit) = (GIV(stackPage)->stackLimit);
	}
	markStackPageMostRecentlyUsed(thePage);

	/* begin restoreCStackStateForCallbackContext: */
	memcpy(GIV(reenterInterpreter), ((void *)((vmCallbackContext->savedReenterInterpreter))), sizeof(jmp_buf));

	/* N.B. siglongjmp is defined as _longjmp on non-win32 platforms.
	   This matches the use of _setjmp in ia32abicc.c. */
	_longjmp((vmCallbackContext->trampoline), (returnTypeOop >> 3));

	/* NOTREACHED */
	return 1;
}