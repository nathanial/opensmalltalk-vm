/* Extracted from interp.c:69988 (function primitiveStoreStackp). */

/*	Atomic store into context stackPointer.
        Also ensures that any newly accessible cells are initialized to nil */

/* StackInterpreterPrimitives>>#primitiveStoreStackp */

static void primitiveStoreStackp(void) {
  sqInt ctxt;
  sqInt i;
  sqInt integerPointer;
  sqInt newStackp;
  usqInt numSlots;
  int onCurrentPage;
  sqInt senderOop;
  sqInt sp;
  sqInt stackp;
  char *theFP;
  StackPage *thePage;

  ctxt = longAt(stackPointer + (1 * BytesPerWord));

  /* begin stackIntegerValue: */
  integerPointer = longAt(stackPointer);
  if ((((integerPointer) & 7) == 1)) {
    newStackp = (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    newStackp = 0;
  }
  if (!((!primFailCode) &&
        (((newStackp >= 0) &&
          (newStackp <=
           (((
              numSlotsOf(ctxt))) -
            CtxtTempFrameStart)))))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

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
  if (/* isStillMarriedContext: */
      (((((fetchPointerofObject(SenderIndex, ctxt))) & 7) == 1)) &&
      (!(isWidowedContext(ctxt)))) {
    /* begin frameOfMarriedContext: */
    senderOop = fetchPointerofObject(SenderIndex, ctxt);
    assert((((senderOop) & 7) == 1));
    theFP = ((char *)(senderOop - (smallIntegerTag())));

    /* begin stackPageFor: */
    thePage = stackPageAtpages(
        pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage),
        pages);
    if (((onCurrentPage = thePage == stackPage)) && (theFP == framePointer)) {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
      return;
    }
    externalDivorceFrameandContext(theFP, ctxt);
    if (onCurrentPage) {
      /* begin setStackPointersFromPage: */
      stackPointer = (stackPage->headSP);
      framePointer = (stackPage->headFP);
    } else {
      assert(stackPage == (stackPageFor(framePointer)));
      markStackPageMostRecentlyUsed(stackPage);
    }
  }

  /* begin fetchStackPointerOf: */
  sp = fetchPointerofObject(StackPointerIndex, ctxt);
  if (!((((sp) & 7) == 1))) {
    stackp = 0;
    goto l1;
  }
  assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(ctxt)));
  stackp = (sp >> 3);
  /* end fetchStackPointerOf: */
l1:

  /* Nil any newly accessible cells */
  for (i = (stackp + 1); i <= newStackp; i += 1) {
    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(ctxt)) && (!(isForwarded(ctxt))));
    assert(validStorePointerUncheckedArgs((i + CtxtTempFrameStart) - 1, ctxt,
                                          nilObj));
    longAtput((void *)((ctxt + BaseHeaderSize) +
                       ((((usqInt)(((i + CtxtTempFrameStart) - 1))
                          << (shiftForWord()))))),
              nilObj);
  }

  /* begin storeStackPointerValue:inContext: */
  assert((ReceiverIndex + newStackp) < (lengthOf(ctxt)));
  assert((isNonImmediate(ctxt)) && (!(isForwarded(ctxt))));
  assert(validStorePointerUncheckedArgs(StackPointerIndex, ctxt,
                                        (((usqInt)newStackp << 3) | 1)));
  longAtput((void *)((ctxt + BaseHeaderSize) +
                     ((((usqInt)(StackPointerIndex) << (shiftForWord()))))),
            (((usqInt)newStackp << 3) | 1));

  /* begin pop: */
  stackPointer += 1 * BytesPerWord;
}