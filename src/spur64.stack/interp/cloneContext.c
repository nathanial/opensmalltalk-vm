/* Extracted from interp.c:65951 (function cloneContext). */

/* StackInterpreterPrimitives>>#cloneContext: */

static NoDbgRegParms usqInt cloneContext(sqInt aContext) {
  usqInt cloned;
  sqInt fieldIndex;
  usqInt frameNumArgs;
  sqInt i;
  usqInt newObj;
  usqInt numBytes;
  usqInt numSlots;
  sqInt senderOop;
  sqInt sp;
  char *spouseFP;
  usqInt sz;
  sqInt valuePointer;

  /* begin numSlotsOf: */
  assert((classIndexOf(aContext)) > (isForwardedObjectClassIndexPun()));
  sz = (((numSlots =
              byteAt((void *)(aContext + (numSlotsFieldByteOffset()))))) ==
                (numSlotsMask())
            ? ((((usqInt)(((
                  sqInt)((usqInt)((longAt((void *)(aContext - BaseHeaderSize))))
                         << 8)))))) >>
                  8
            : numSlots);

  /* begin eeInstantiateMethodContextSlots: */
  /* begin allocateNewSpaceSlots:format:classIndex: */
  if (sz >= (numSlotsMask())) {
    if (sz > 0xFFFFFFFFU) {
      cloned = null;
      goto l1;
    }
    newObj = freeStart + BaseHeaderSize;
    numBytes = (BaseHeaderSize + BaseHeaderSize) + (sz * BytesPerOop);
  } else {
    newObj = freeStart;
    numBytes = BaseHeaderSize + ((sz < 1 ? 8 /* allocationUnit */
                                         : sz * BytesPerOop));
  }
  if ((freeStart + numBytes) > scavengeThreshold) {
    if (!needGCFlag) {
      /* begin scheduleScavenge */
      needGCFlag = 1;
      forceInterruptCheck();
    }
    if ((freeStart + numBytes) > (((eden).limit))) {
      error("no room in eden for allocateNewSpaceSlots:format:classIndex:");
      cloned = 0;
      goto l1;
    }
  }
  if (sz >= (numSlotsMask())) {
    longAtput((void *)(freeStart), sz);
    longAtput((void *)(freeStart + 4),
              ((sqInt)((usqInt)((numSlotsMask())) << (numSlotsHalfShift()))));
    long64Atput(
        (void *)(newObj),
        ((((((usqLong)(numSlotsMask()))) << (numSlotsFullShift()))) +
         ((((usqInt)((indexablePointersFormat())) << (formatShift()))))) +
            ClassMethodContextCompactIndex);
  } else {
    long64Atput(
        (void *)(newObj),
        ((((((usqLong)sz)) << (numSlotsFullShift()))) +
         ((((usqInt)((indexablePointersFormat())) << (formatShift()))))) +
            ClassMethodContextCompactIndex);
  }

  /* for header parsing we put a saturated slot count in the prepended overflow
   * size word */
  assert((numBytes % (allocationUnit())) == 0);
  assert((newObj % (allocationUnit())) == 0);
  freeStart += numBytes;
  cloned = newObj;
  /* end eeInstantiateMethodContextSlots: */
l1:
  assert(!((cloned == 0)));
  for (i = 0; i <= StackPointerIndex; i += 1) {
    valuePointer = externalInstVarofContext(i, aContext);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(cloned)) && (!(isForwarded(cloned))));
    assert(validStorePointerUncheckedArgs(i, cloned, valuePointer));
    longAtput((void *)((cloned + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              valuePointer);
  }
  for (i = MethodIndex; i <= ReceiverIndex; i += 1) {
    valuePointer = longAt((void *)((aContext + BaseHeaderSize) +
                                   ((((usqInt)(i) << (shiftForWord()))))));

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(cloned)) && (!(isForwarded(cloned))));
    assert(validStorePointerUncheckedArgs(i, cloned, valuePointer));
    longAtput((void *)((cloned + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              valuePointer);
  }
  if (/* isStillMarriedContext: */
      (((((longAt((void *)((aContext + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
         7) == 1)) &&
      (!(isWidowedContext(aContext)))) {
    /* begin frameOfMarriedContext: */
    senderOop =
        longAt((void *)((aContext + BaseHeaderSize) +
                        ((((usqInt)(SenderIndex) << (shiftForWord()))))));
    assert((((senderOop) & 7) == 1));
    spouseFP = ((char *)(senderOop - (smallIntegerTag())));
    sp = (stackPointerIndexForFrame(spouseFP)) - 1;
    for (i = 0; i <= sp; i += 1) {
      fieldIndex = i + CtxtTempFrameStart;
      valuePointer = /* temporary:in: */
          (i < ((frameNumArgs = byteAt((spouseFP + FoxFrameFlags) + 1)))
               ? longAt((void *)((spouseFP + FoxCallerSavedIP) +
                                 ((frameNumArgs - i) * BytesPerWord)))
               : longAt((void *)(((spouseFP + FoxReceiver) - BytesPerWord) +
                                 ((frameNumArgs - i) * BytesPerWord))));

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(cloned)) && (!(isForwarded(cloned))));
      assert(validStorePointerUncheckedArgs(fieldIndex, cloned, valuePointer));
      longAtput((void *)((cloned + BaseHeaderSize) +
                         ((((usqInt)(fieldIndex) << (shiftForWord()))))),
                valuePointer);
    }
  } else {
    sp = (fetchStackPointerOf(aContext)) - 1;
    for (i = 0; i <= sp; i += 1) {
      fieldIndex = i + CtxtTempFrameStart;
      valuePointer = longAt((void *)((aContext + BaseHeaderSize) +
                                     ((((usqInt)((i + CtxtTempFrameStart))
                                        << (shiftForWord()))))));

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(cloned)) && (!(isForwarded(cloned))));
      assert(validStorePointerUncheckedArgs(fieldIndex, cloned, valuePointer));
      longAtput((void *)((cloned + BaseHeaderSize) +
                         ((((usqInt)(fieldIndex) << (shiftForWord()))))),
                valuePointer);
    }
  }
  return cloned;
}