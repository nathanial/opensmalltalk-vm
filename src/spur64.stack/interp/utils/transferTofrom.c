/* Extracted from interp.c:65167 (function transferTofrom). */

/*	Record a process to be awoken on the next interpreter cycle.
        Note: It is currently a fatal VM error if there is no runnable process
        (indicated by newProcOrNil being nil), but the right thing to do is to
        enter a wait state until the next event/expired delay/available input,
        etc.  */

/* StackInterpreter>>#transferTo:from: */

static sqInt transferTofrom(sqInt newProcOrNil, sqInt sourceCode) {
  sqInt activeContext;
  StackPage *lastUsedPage;
  sqInt methodHeader;
  sqInt newContext;
  StackPage *newPage;
  sqInt oldProc;
  char *ptr;
  sqInt sched;
  sqInt senderOop;
  char *sp;
  char *theFrame;
  StackPage *thePage;
  char *toDoLimit;
  sqInt top;

  statProcessSwitch += 1;

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

  /* begin assertValidExternalFrameWithIP: */
  assertValidExecutionPointersimbarline(
      instructionPointer + 1, framePointer, stackPointer,
      1 /* (isMachineCodeFrame: not) */, __LINE__);
  for (ptr = (stackPointer + BytesPerWord); ptr <= (framePointer + FoxReceiver);
       ptr += BytesPerWord) {
    assert(addressCouldBeOop(longAt(ptr)));
  }

  /* skip pushed instructionPointer */
  assert(isOopCompiledMethod(frameMethodObject(framePointer)));
  if (byteAt((framePointer + FoxFrameFlags) + 2)) {
    assert(!((isForwarded(frameContext(framePointer)))));
  }
  toDoLimit =
      framePointer + ((FoxCallerSavedIP + BytesPerWord) +
                      ((((usqInt)((byteAt((framePointer + FoxFrameFlags) + 1)))
                         << (shiftForWord())))));
  for (ptr = ((framePointer + FoxCallerSavedIP) + BytesPerWord);
       ptr <= toDoLimit; ptr += BytesPerWord) {
    assert(addressCouldBeOop(longAt(ptr)));
  }
  sched = fetchPointerofObject(
      ValueIndex,
      fetchPointerofObject(SchedulerAssociation, specialObjectsOop));
  oldProc = fetchPointerofObject(ActiveProcessIndex, sched);

  /* begin recordContextSwitchFrom:in: */
  /* begin recordTrace:thing:source: */
  if (TraceLog) {
    traceLog[traceLogIndex] = TraceContextSwitch;
    traceLog[traceLogIndex + 1] = oldProc;
    traceLog[traceLogIndex + 2] = sourceCode;
    traceLogIndex = (traceLogIndex + 3) % TraceBufferSize;
  }

  /* begin ensureFrameIsMarried:SP: */
  if (byteAt((framePointer + FoxFrameFlags) + 2)) {
    assert(isContext(frameContext(framePointer)));
    activeContext = longAt(framePointer + FoxThisContext);
    goto l1;
  }
  activeContext = marryFrameSP(framePointer, stackPointer + BytesPerWord);
  /* end ensureFrameIsMarried:SP: */
l1:

  storePointerofObjectwithValue(SuspendedContextIndex, oldProc, activeContext);
  if (!newProcOrNil) {
    error("scheduler could not find a runnable process");
  }

  storePointerofObjectwithValue(ActiveProcessIndex, sched, newProcOrNil);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(newProcOrNil)) && (!(isForwarded(newProcOrNil))));
  assert(validStorePointerUncheckedArgs(MyListIndex, newProcOrNil, nilObj));
  longAtput((void *)((newProcOrNil + BaseHeaderSize) +
                     ((((usqInt)(MyListIndex) << (shiftForWord()))))),
            nilObj);

  /* begin externalSetStackPageAndPointersForSuspendedContextOfProcess: */
  newContext = fetchPointerofObject(SuspendedContextIndex, newProcOrNil);
  assert(isContext(newContext));
  if (((((fetchPointerofObject(SenderIndex, newContext))) & 7) == 1)) {
    assert(checkIsStillMarriedContextcurrentFP(newContext, framePointer));
  }

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(newProcOrNil)) && (!(isForwarded(newProcOrNil))));
  assert(validStorePointerUncheckedArgs(SuspendedContextIndex, newProcOrNil,
                                        nilObj));
  longAtput((void *)((newProcOrNil + BaseHeaderSize) +
                     ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))),
            nilObj);
  if (/* isStillMarriedContext: */
      (((((fetchPointerofObject(SenderIndex, newContext))) & 7) == 1)) &&
      (!(isWidowedContext(newContext)))) {
    /* begin frameOfMarriedContext: */
    senderOop = fetchPointerofObject(SenderIndex, newContext);
    assert((((senderOop) & 7) == 1));
    theFrame = ((char *)(senderOop - (smallIntegerTag())));

    /* begin stackPageFor: */
    thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                   theFrame, stackMemory, bytesPerPage),
                               pages);
    if (theFrame != ((thePage->headFP))) {
      /* begin newStackPage */
      newPage = (mostRecentlyUsedPage->nextPage);
      if (!((newPage->baseFP))) {
        goto l3;
      }
      divorceFramesIn(newPage);
      /* end newStackPage */
    l3:
      moveFramesInthroughtoPage(
          thePage, findFrameAboveinPage(theFrame, thePage), newPage);

      /* begin markStackPageLeastMostRecentlyUsed: */
      assert(newPage == ((mostRecentlyUsedPage->nextPage)));
      lastUsedPage = (newPage->nextPage);
      while (((lastUsedPage->baseFP)) == 0) {
        lastUsedPage = (lastUsedPage->nextPage);
      }
      if (((lastUsedPage->nextPage)) == newPage) {
        goto l2;
      }
      (((newPage->prevPage))->nextPage = (newPage->nextPage));
      (((newPage->nextPage))->prevPage = (newPage->prevPage));
      (((lastUsedPage->prevPage))->nextPage = newPage);
      (newPage->prevPage = (lastUsedPage->prevPage));
      (newPage->nextPage = lastUsedPage);
      (lastUsedPage->prevPage = newPage);
      assert(pageListIsWellFormed());
      /* end markStackPageLeastMostRecentlyUsed: */
    l2:;
    }

    /* explicit assignment of suspendedContext can cause switch to interior
     * frame. */
    assert(((thePage->headFP)) == theFrame);
  } else {
    thePage = makeBaseFrameFor(newContext);
    theFrame = (thePage->baseFP);
  }

  /* begin setStackPageAndLimit: */
  assert(thePage);
  stackPage = thePage;
  if (stackLimit != (((char *)(((usqInt)-1))))) {
    stackLimit = (stackPage->stackLimit);
  }
  markStackPageMostRecentlyUsed(thePage);

  /* begin setStackPointersFromPage: */
  stackPointer = (thePage->headSP);
  framePointer = (thePage->headFP);

  /* begin setMethod: */
  method = longAt(framePointer + FoxMethod);
  assert(isOopCompiledMethod(method));

  /* begin methodUsesAlternateBytecodeSet: */
  /* begin methodHeaderOf: */
  assert(isCompiledMethod(method));
  methodHeader = fetchPointerofObject(HeaderIndex, method);
  if ((((sqLong)methodHeader)) < 0) {
    bytecodeSetSelector = 0x100;
  } else {
    bytecodeSetSelector = 0;
  }

  /* begin popStack */
  top = longAt(stackPointer);
  stackPointer += BytesPerWord;
  instructionPointer = top;

  /* begin assertValidExecutionPointe:r:s: */
  assertValidExecutionPointersimbarline(
      instructionPointer, framePointer, stackPointer,
      1 /* (isMachineCodeFrame: not) */, __LINE__);
  return 0;
}