/* Extracted from interp.c:61894 (function reestablishContextPriorToCallback). */

/*	callbackContext is an activation of
	invokeCallback:[stack:registers:jmpbuf:]. Its sender is the VM's state
	prior to the callback. Reestablish that state,
	and mark calloutContext as dead. */

	/* StackInterpreter>>#reestablishContextPriorToCallback: */

sqInt
reestablishContextPriorToCallback(sqInt callbackContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt calloutContext;
    sqInt senderOop;
    char *sp;
    char *theFP;
    StackPage *thePage;
    sqInt top;

	if (!(isLiveContext(callbackContext))) {
		return 0;
	}
	calloutContext = externalInstVarofContext(SenderIndex, callbackContext);
	if (!(isLiveContext(calloutContext))) {
		return 0;
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

	/* Mark callbackContext as dead; the common case is that it is the current frame.
	   We go the extra mile for the debugger. */
	if ((!((longAt((void *)((callbackContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))))) & (tagMask())))) {
		/* begin markContextAsDead: */
		assert(isContext(callbackContext));
		assert((isNonImmediate(callbackContext))
		 && (!(isForwarded(callbackContext))));
		assert(validStorePointerUncheckedArgs(SenderIndex, callbackContext, GIV(nilObj)));
		longAtput((void *)((callbackContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),GIV(nilObj));

		/* begin storePointerUnchecked:ofObject:withValue: */
		assert((isNonImmediate(callbackContext))
		 && (!(isForwarded(callbackContext))));
		assert(validStorePointerUncheckedArgs(InstructionPointerIndex, callbackContext, GIV(nilObj)));
		longAtput((void *)((callbackContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),GIV(nilObj));
	}
	else {
		/* begin frameOfMarriedContext: */
		senderOop = longAt((void *)((callbackContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
		assert((((senderOop) & 7) == 1));
		theFP = ((char *)(senderOop - (smallIntegerTag())));
		if (GIV(framePointer) == theFP) {
			if (longAt(theFP + FoxSavedFP)) {
				GIV(instructionPointer) = longAt(GIV(framePointer) + FoxCallerSavedIP);
				GIV(stackPointer) = (GIV(framePointer) + ((FoxCallerSavedIP + BytesPerWord) + ((((usqInt)((byteAt((GIV(framePointer) + FoxFrameFlags) + 1))) << (shiftForWord())))))) + BytesPerWord;
				GIV(framePointer) = ((char *)(longAt(GIV(framePointer) + FoxSavedFP)));
				return 1;
			}
			else {
				freeStackPage(GIV(stackPage));
			}
		}
		else {
			externalDivorceFrameandContext(theFP, callbackContext);

			/* begin markContextAsDead: */
			assert(isContext(callbackContext));
			assert((isNonImmediate(callbackContext))
			 && (!(isForwarded(callbackContext))));
			assert(validStorePointerUncheckedArgs(SenderIndex, callbackContext, GIV(nilObj)));
			longAtput((void *)((callbackContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),GIV(nilObj));

			/* begin storePointerUnchecked:ofObject:withValue: */
			assert((isNonImmediate(callbackContext))
			 && (!(isForwarded(callbackContext))));
			assert(validStorePointerUncheckedArgs(InstructionPointerIndex, callbackContext, GIV(nilObj)));
			longAtput((void *)((callbackContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),GIV(nilObj));
		}
	}

	/* Make the calloutContext the active frame.  The case where calloutContext
	   is immediately below callbackContext on the same page is handled above. */
	if (/* isStillMarriedContext: */
		(((((longAt((void *)((calloutContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
	 && (!(isWidowedContext(calloutContext)))) {
		/* begin frameOfMarriedContext: */
		senderOop = longAt((void *)((calloutContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
		assert((((senderOop) & 7) == 1));
		theFP = ((char *)(senderOop - (smallIntegerTag())));

		/* begin stackPageFor: */
		thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(theFP, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));

		/* findSPOf:on: points to the word beneath the instructionPointer, but
		   there is no instructionPointer on the top frame of the current page. */
		assert(thePage != GIV(stackPage));
		GIV(stackPointer) = (findSPOfon(theFP, thePage)) - BytesPerWord;
		GIV(framePointer) = theFP;
		assert(GIV(stackPointer) < GIV(framePointer));
	}
	else {
		thePage = makeBaseFrameFor(calloutContext);

		/* begin setStackPointersFromPage: */
		GIV(stackPointer) = (thePage->headSP);
		GIV(framePointer) = (thePage->headFP);
	}

	/* begin popStack */
	top = longAt(GIV(stackPointer));
	GIV(stackPointer) += BytesPerWord;
	GIV(instructionPointer) = top;

	/* begin setStackPageAndLimit: */
	assert(thePage);
	GIV(stackPage) = thePage;
	if (GIV(stackLimit) != (((char *) (((usqInt) -1))))) {
		GIV(stackLimit) = (GIV(stackPage)->stackLimit);
	}
	markStackPageMostRecentlyUsed(thePage);
	return 1;
}