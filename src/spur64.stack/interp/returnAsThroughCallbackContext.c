/* Extracted from interp.c:62436 (function returnAsThroughCallbackContext). */

/*	callbackMethodContext is an activation of
	invokeCallback:[stack:registers:jmpbuf:]. Its sender is the VM's state
	prior to the callback. Reestablish that state (via longjmp),
	and mark callbackMethodContext as dead. */

	/* StackInterpreter>>#returnAs:ThroughCallback:Context: */

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

	assert(primFailCode == 0);
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
		traceLog[traceLogIndex] = TraceVMCallbackReturn;
		traceLog[traceLogIndex + 1] = returnTypeOop;
		traceLog[traceLogIndex + 2] = 0;
		traceLogIndex = (traceLogIndex + 3) % TraceBufferSize;
	}

	/* We're about to leave this stack page; must save the current frame's instructionPointer. */

	/* begin push: */
	longAtput((sp = stackPointer - BytesPerWord),instructionPointer);
	stackPointer = sp;

	/* begin externalWriteBackHeadFramePointers */
	assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
	assert(stackPage == (mostRecentlyUsedPage));
	assert(!((isFree(stackPage))));

	/* begin setHeadFP:andSP:inPage: */
	assert(stackPointer < framePointer);
	assert((stackPointer < ((stackPage->baseAddress)))
	 && (stackPointer > (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
	assert((framePointer < ((stackPage->baseAddress)))
	 && (framePointer > (((stackPage->realStackLimit)) - ((LargeContextSlots * BytesPerOop) / 2))));
	(stackPage->headFP = framePointer);
	(stackPage->headSP = stackPointer);
	assert(pageListIsWellFormed());

	/* Mark callbackMethodContext as dead; the common case is that it is the current frame.
	   We go the extra mile for the debugger. */
	if ((!((longAt((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))))) & (tagMask())))) {
		assert(((debugCallbackPath = debugCallbackPath | 4)) > 0);

		/* begin markContextAsDead: */
		assert(isContext(callbackMethodContext));
		assert((isNonImmediate(callbackMethodContext))
		 && (!(isForwarded(callbackMethodContext))));
		assert(validStorePointerUncheckedArgs(SenderIndex, callbackMethodContext, nilObj));
		longAtput((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),nilObj);

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(callbackMethodContext))
		 && (!(isForwarded(callbackMethodContext))));
		assert(validStorePointerUncheckedArgs(InstructionPointerIndex, callbackMethodContext, nilObj));
		longAtput((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),nilObj);
	}
	else {
		assert(((debugCallbackPath = debugCallbackPath | 8)) > 0);

		/* begin frameOfMarriedContext: */
		senderOop = longAt((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
		assert((((senderOop) & 7) == 1));
		theFP = ((char *)(senderOop - (smallIntegerTag())));
		assert((frameReceiver(theFP)) == (splObj(ClassAlien)));
		if (framePointer == theFP) {
			assert(((debugCallbackPath = debugCallbackPath | 16)) > 0);
			if (longAt(theFP + FoxSavedFP)) {
				assert(((debugCallbackPath = debugCallbackPath | 32)) > 0);
				instructionPointer = longAt(theFP + FoxCallerSavedIP);
				stackPointer = (theFP + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord;
				framePointer = ((char *)(longAt(theFP + FoxSavedFP)));

				/* begin setMethod: */
				method = longAt(framePointer + FoxMethod);
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

				/* begin restoreCStackStateForCallbackContext: */
				memcpy(reenterInterpreter, ((void *)((vmCallbackContext->savedReenterInterpreter))), sizeof(jmp_buf));
				assertValidExecutionPointersimbarline(instructionPointer, framePointer, stackPointer, 1 /* (isMachineCodeFrame: not) */, __LINE__);

				/* N.B. siglongjmp is defines as _longjmp on non-win32 platforms.
				   This matches the use of _setjmp in ia32abicc.c. */
				_longjmp((vmCallbackContext->trampoline), (returnTypeOop >> 3));
				return 1;
			}

			/* calloutMethodContext is immediately below on the same page.  Make it current. */
			freeStackPage(stackPage);
		}
		else {
			assert(((debugCallbackPath = debugCallbackPath | 64)) > 0);
			externalDivorceFrameandContext(theFP, callbackMethodContext);

			/* begin markContextAsDead: */
			assert(isContext(callbackMethodContext));
			assert((isNonImmediate(callbackMethodContext))
			 && (!(isForwarded(callbackMethodContext))));
			assert(validStorePointerUncheckedArgs(SenderIndex, callbackMethodContext, nilObj));
			longAtput((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),nilObj);

			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(callbackMethodContext))
			 && (!(isForwarded(callbackMethodContext))));
			assert(validStorePointerUncheckedArgs(InstructionPointerIndex, callbackMethodContext, nilObj));
			longAtput((void *)((callbackMethodContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),nilObj);
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
		thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage), pages);

		/* findSPOf:on: points to the word beneath the instructionPointer, but
		   there is no instructionPointer on the top frame of the current page. */
		assert(thePage != stackPage);
		stackPointer = (((thePage->headFP)) == theFP
					? (thePage->headSP)
					: (findSPOfon(theFP, thePage)) - BytesPerWord);
		framePointer = theFP;
		assert(stackPointer < framePointer);
	}
	else {
		assert(((debugCallbackPath = debugCallbackPath | 0x100)) > 0);
		thePage = makeBaseFrameFor(calloutMethodContext);

		/* begin setStackPointersFromPage: */
		stackPointer = (thePage->headSP);
		framePointer = (thePage->headFP);
	}

	/* begin popStack */
	top = longAt(stackPointer);
	stackPointer += BytesPerWord;
	instructionPointer = top;

	/* begin setMethod: */
	method = longAt((void *)((calloutMethodContext + BaseHeaderSize) + ((((usqInt)(MethodIndex) << (shiftForWord()))))));
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

	/* begin setStackPageAndLimit: */
	assert(thePage);
	stackPage = thePage;
	if (stackLimit != (((char *) (((usqInt) -1))))) {
		stackLimit = (stackPage->stackLimit);
	}
	markStackPageMostRecentlyUsed(thePage);

	/* begin restoreCStackStateForCallbackContext: */
	memcpy(reenterInterpreter, ((void *)((vmCallbackContext->savedReenterInterpreter))), sizeof(jmp_buf));

	/* N.B. siglongjmp is defined as _longjmp on non-win32 platforms.
	   This matches the use of _setjmp in ia32abicc.c. */
	_longjmp((vmCallbackContext->trampoline), (returnTypeOop >> 3));

	/* NOTREACHED */
	return 1;
}