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
  sched =
      longAt((void *)(((longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                                        ((((usqInt)(SchedulerAssociation)
                                           << (shiftForWord()))))))) +
                       BaseHeaderSize) +
                      ((((usqInt)(ValueIndex) << (shiftForWord()))))));
  oldProc =
      longAt((void *)((sched + BaseHeaderSize) +
                      ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))));

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

  /* begin storePointer:ofObject:withValue: */
  assert(validStorePointerArgs(SuspendedContextIndex, oldProc, activeContext));
  assert(isNonImmediate(oldProc));
  if (oopisGreaterThanOrEqualTo(oldProc, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(activeContext & (tagMask())))) &&
        (oopisLessThan(activeContext, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(oldProc + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(oldProc);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((oldProc + BaseHeaderSize) +
                     ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))),
            activeContext);
  if (!newProcOrNil) {
    error("scheduler could not find a runnable process");
  }

  /* begin storePointer:ofObject:withValue: */
  assert(validStorePointerArgs(ActiveProcessIndex, sched, newProcOrNil));
  assert(isNonImmediate(sched));
  if (oopisGreaterThanOrEqualTo(sched, oldSpaceStart)) {
    if (/* isYoung: */
        ((!(newProcOrNil & (tagMask())))) &&
        (oopisLessThan(newProcOrNil, oldSpaceStart))) {
      /* begin possibleRootStoreInto: */
      if (!((byteAt((void *)(sched + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift())))) {
        remember(sched);
      }
    }
  }

  /* most stores into young objects */
  longAtput((void *)((sched + BaseHeaderSize) +
                     ((((usqInt)(ActiveProcessIndex) << (shiftForWord()))))),
            newProcOrNil);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(newProcOrNil)) && (!(isForwarded(newProcOrNil))));
  assert(validStorePointerUncheckedArgs(MyListIndex, newProcOrNil, nilObj));
  longAtput((void *)((newProcOrNil + BaseHeaderSize) +
                     ((((usqInt)(MyListIndex) << (shiftForWord()))))),
            nilObj);

  /* begin externalSetStackPageAndPointersForSuspendedContextOfProcess: */
  newContext = longAt(
      (void *)((newProcOrNil + BaseHeaderSize) +
               ((((usqInt)(SuspendedContextIndex) << (shiftForWord()))))));
  assert(isContext(newContext));
  if (((((longAt((void *)((newContext + BaseHeaderSize) +
                          ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
        7) == 1)) {
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
      (((((longAt((void *)((newContext + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
         7) == 1)) &&
      (!(isWidowedContext(newContext)))) {
    /* begin frameOfMarriedContext: */
    senderOop =
        longAt((void *)((newContext + BaseHeaderSize) +
                        ((((usqInt)(SenderIndex) << (shiftForWord()))))));
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
  methodHeader =
      longAt((void *)((method + BaseHeaderSize) +
                      ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
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