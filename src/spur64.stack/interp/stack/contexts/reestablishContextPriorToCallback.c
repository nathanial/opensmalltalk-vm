/* Extracted from interp.c:61894 (function reestablishContextPriorToCallback).
 */

/*	callbackContext is an activation of
        invokeCallback:[stack:registers:jmpbuf:]. Its sender is the VM's state
        prior to the callback. Reestablish that state,
        and mark calloutContext as dead. */

/* StackInterpreter>>#reestablishContextPriorToCallback: */

sqInt reestablishContextPriorToCallback(sqInt callbackContext) {
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

  /* We're about to leave this stack page; must save the current frame's
   * instructionPointer. */

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), instructionPointer);
  stackPointer = sp;

  /* begin externalWriteBackHeadFramePointers */
  assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
  assert(stackPage == (mostRecentlyUsedPage));
  assert(!((isFree(stackPage))));

  /* begin setHeadFP:andSP:inPage: */
  assert(stackPointer < framePointer);
  assert((stackPointer < ((stackPage->baseAddress))) &&
         (stackPointer >
          (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
  assert((framePointer < ((stackPage->baseAddress))) &&
         (framePointer > (((stackPage->realStackLimit)) -
                          ((LargeContextSlots * BytesPerOop) / 2))));
  (stackPage->headFP = framePointer);
  (stackPage->headSP = stackPointer);
  assert(pageListIsWellFormed());

  /* Mark callbackContext as dead; the common case is that it is the current
     frame. We go the extra mile for the debugger. */
  if ((!((fetchPointerofObject(SenderIndex, callbackContext)) & (tagMask())))) {
    /* begin markContextAsDead: */
    assert(isContext(callbackContext));
    assert((isNonImmediate(callbackContext)) &&
           (!(isForwarded(callbackContext))));
    assert(
        validStorePointerUncheckedArgs(SenderIndex, callbackContext, nilObj));
    longAtput((void *)((callbackContext + BaseHeaderSize) +
                       ((((usqInt)(SenderIndex) << (shiftForWord()))))),
              nilObj);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(callbackContext)) &&
           (!(isForwarded(callbackContext))));
    assert(validStorePointerUncheckedArgs(InstructionPointerIndex,
                                          callbackContext, nilObj));
    longAtput(
        (void *)((callbackContext + BaseHeaderSize) +
                 ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),
        nilObj);
  } else {
    /* begin frameOfMarriedContext: */
    senderOop = fetchPointerofObject(SenderIndex, callbackContext);
    assert((((senderOop) & 7) == 1));
    theFP = ((char *)(senderOop - (smallIntegerTag())));
    if (framePointer == theFP) {
      if (longAt(theFP + FoxSavedFP)) {
        instructionPointer = longAt(framePointer + FoxCallerSavedIP);
        stackPointer =
            (framePointer +
             ((FoxCallerSavedIP + BytesPerWord) +
              ((((usqInt)((byteAt((framePointer + FoxFrameFlags) + 1)))
                 << (shiftForWord())))))) +
            BytesPerWord;
        framePointer = ((char *)(longAt(framePointer + FoxSavedFP)));
        return 1;
      } else {
        freeStackPage(stackPage);
      }
    } else {
      externalDivorceFrameandContext(theFP, callbackContext);

      /* begin markContextAsDead: */
      assert(isContext(callbackContext));
      assert((isNonImmediate(callbackContext)) &&
             (!(isForwarded(callbackContext))));
      assert(
          validStorePointerUncheckedArgs(SenderIndex, callbackContext, nilObj));
      longAtput((void *)((callbackContext + BaseHeaderSize) +
                         ((((usqInt)(SenderIndex) << (shiftForWord()))))),
                nilObj);

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(callbackContext)) &&
             (!(isForwarded(callbackContext))));
      assert(validStorePointerUncheckedArgs(InstructionPointerIndex,
                                            callbackContext, nilObj));
      longAtput(
          (void *)((callbackContext + BaseHeaderSize) +
                   ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),
          nilObj);
    }
  }

  /* Make the calloutContext the active frame.  The case where calloutContext
     is immediately below callbackContext on the same page is handled above. */
  if (/* isStillMarriedContext: */
      (((((fetchPointerofObject(SenderIndex, calloutContext))) & 7) == 1)) &&
      (!(isWidowedContext(calloutContext)))) {
    /* begin frameOfMarriedContext: */
    senderOop = fetchPointerofObject(SenderIndex, calloutContext);
    assert((((senderOop) & 7) == 1));
    theFP = ((char *)(senderOop - (smallIntegerTag())));

    /* begin stackPageFor: */
    thePage = stackPageAtpages(
        pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage),
        pages);

    /* findSPOf:on: points to the word beneath the instructionPointer, but
       there is no instructionPointer on the top frame of the current page. */
    assert(thePage != stackPage);
    stackPointer = (findSPOfon(theFP, thePage)) - BytesPerWord;
    framePointer = theFP;
    assert(stackPointer < framePointer);
  } else {
    thePage = makeBaseFrameFor(calloutContext);

    /* begin setStackPointersFromPage: */
    stackPointer = (thePage->headSP);
    framePointer = (thePage->headFP);
  }

  /* begin popStack */
  top = longAt(stackPointer);
  stackPointer += BytesPerWord;
  instructionPointer = top;

  /* begin setStackPageAndLimit: */
  assert(thePage);
  stackPage = thePage;
  if (stackLimit != (((char *)(((usqInt)-1))))) {
    stackLimit = (stackPage->stackLimit);
  }
  markStackPageMostRecentlyUsed(thePage);
  return 1;
}