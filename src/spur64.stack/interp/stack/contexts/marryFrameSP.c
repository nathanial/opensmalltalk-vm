/* Extracted from interp.c:57469 (function marryFrameSP). */

/*	Marry an unmarried frame. This means creating a spouse context
        initialized with a subset of the frame's state (state through the last
        argument) that references the frame. */

/* StackInterpreter>>#marryFrame:SP: */

static usqInt marryFrameSP(char *theFP, char *theSP) {
  sqInt closureOrNil;
  sqInt i;
  sqInt methodHeader;
  usqInt newObj;
  usqInt numArgs;
  usqInt numBytes;
  sqInt numSlots;
  usqInt numStack;
  usqInt theContext;
  sqInt valuePointer;

  assert(!(frameHasContext(theFP)));

  /* The SP is expected to be pointing at the last oop on the stack, not at the
   * pc */
  assert(addressCouldBeOop(longAt(theSP)));

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(longAt(theFP + FoxMethod)));
  methodHeader =
      fetchPointerofObject(HeaderIndex, longAt(theFP + FoxMethod));

  /* Decide how much of the stack to preserve in widowed contexts.  Preserving
     too much state will potentially hold onto garbage.  Holding onto too little
     may mean that a dead context isn't informative enough in a debugging
     situation.  If copyTemps is false (as it is in the default closure
     implementation) compromise, retaining only the arguments with no
     temporaries.  Note that we still set the stack pointer to its current
     value, but stack contents other than the arguments are nil. */
  if (byteAt((theFP + FoxFrameFlags) + 3)) {
    numArgs = byteAt((theFP + FoxFrameFlags) + 1);
    closureOrNil =
        longAt(theFP + ((FoxCallerSavedIP + BytesPerWord) +
                        ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1)))
                           << (shiftForWord()))))));
  } else {
    numArgs = (((usqInt)(methodHeader)) >> MethodHeaderArgCountShift) & 15;
    closureOrNil = nilObj;
  }
  numStack = (((usqInt)(((theFP + FoxReceiver) - theSP))) >> (shiftForWord())) +
             (byteAt((theFP + FoxFrameFlags) + 1));
  numSlots =
      (methodHeader & LargeContextBit ? LargeContextSlots : SmallContextSlots);

  /* begin eeInstantiateMethodContextSlots: */
  /* begin allocateNewSpaceSlots:format:classIndex: */
  if (numSlots >= (numSlotsMask())) {
    if (numSlots > 0xFFFFFFFFU) {
      theContext = null;
      goto l1;
    }
    newObj = freeStart + BaseHeaderSize;
    numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
  } else {
    newObj = freeStart;
    numBytes = BaseHeaderSize + ((numSlots < 1 ? 8 /* allocationUnit */
                                               : numSlots * BytesPerOop));
  }
  if ((freeStart + numBytes) > scavengeThreshold) {
    if (!needGCFlag) {
      /* begin scheduleScavenge */
      needGCFlag = 1;
      forceInterruptCheck();
    }
    if ((freeStart + numBytes) > (((eden).limit))) {
      error("no room in eden for allocateNewSpaceSlots:format:classIndex:");
      theContext = 0;
      goto l1;
    }
  }
  if (numSlots >= (numSlotsMask())) {
    longAtput((void *)(freeStart), numSlots);
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
        ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
         ((((usqInt)((indexablePointersFormat())) << (formatShift()))))) +
            ClassMethodContextCompactIndex);
  }

  /* for header parsing we put a saturated slot count in the prepended overflow
   * size word */
  assert((numBytes % (allocationUnit())) == 0);
  assert((newObj % (allocationUnit())) == 0);
  freeStart += numBytes;
  theContext = newObj;
  /* end eeInstantiateMethodContextSlots: */
l1:

  /* Mark context as married by setting its sender to the frame pointer plus
     SmallInteger tags and the InstructionPointer to the saved fp (which ensures
     correct alignment w.r.t. the frame when we check for validity) */
  /* begin withSmallIntegerTags: */
  assert(((oopForPointer(theFP)) & (BytesPerWord - 1)) == 0);
  valuePointer = (oopForPointer(theFP)) + (smallIntegerTag());

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(theContext)) && (!(isForwarded(theContext))));
  assert(validStorePointerUncheckedArgs(SenderIndex, theContext, valuePointer));
  longAtput((void *)((theContext + BaseHeaderSize) +
                     ((((usqInt)(SenderIndex) << (shiftForWord()))))),
            valuePointer);
  /* begin withSmallIntegerTags: */
  assert(((oopForPointer(((char *)(longAt(theFP + FoxSavedFP))))) &
          (BytesPerWord - 1)) == 0);
  valuePointer = (oopForPointer(((char *)(longAt(theFP + FoxSavedFP))))) +
                 (smallIntegerTag());

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(theContext)) && (!(isForwarded(theContext))));
  assert(validStorePointerUncheckedArgs(InstructionPointerIndex, theContext,
                                        valuePointer));
  longAtput(
      (void *)((theContext + BaseHeaderSize) +
               ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),
      valuePointer);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(theContext)) && (!(isForwarded(theContext))));
  assert(validStorePointerUncheckedArgs(StackPointerIndex, theContext,
                                        ((numStack << 3) | 1)));
  longAtput((void *)((theContext + BaseHeaderSize) +
                     ((((usqInt)(StackPointerIndex) << (shiftForWord()))))),
            ((numStack << 3) | 1));

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(theContext)) && (!(isForwarded(theContext))));
  assert(validStorePointerUncheckedArgs(MethodIndex, theContext,
                                        longAt(theFP + FoxMethod)));
  longAtput((void *)((theContext + BaseHeaderSize) +
                     ((((usqInt)(MethodIndex) << (shiftForWord()))))),
            longAt(theFP + FoxMethod));

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(theContext)) && (!(isForwarded(theContext))));
  assert(
      validStorePointerUncheckedArgs(ClosureIndex, theContext, closureOrNil));
  longAtput((void *)((theContext + BaseHeaderSize) +
                     ((((usqInt)(ClosureIndex) << (shiftForWord()))))),
            closureOrNil);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(theContext)) && (!(isForwarded(theContext))));
  assert(validStorePointerUncheckedArgs(ReceiverIndex, theContext,
                                        longAt(theFP + FoxReceiver)));
  longAtput((void *)((theContext + BaseHeaderSize) +
                     ((((usqInt)(ReceiverIndex) << (shiftForWord()))))),
            longAt(theFP + FoxReceiver));
  for (i = 1; i <= numArgs; i += 1) {
    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(theContext)) && (!(isForwarded(theContext))));
    assert(validStorePointerUncheckedArgs(
        ReceiverIndex + i, theContext,
        longAt((void *)((theFP + FoxCallerSavedIP) +
                        ((numArgs - (i - 1)) * BytesPerWord)))));
    longAtput((void *)((theContext + BaseHeaderSize) +
                       ((((usqInt)((ReceiverIndex + i)) << (shiftForWord()))))),
              longAt((void *)((theFP + FoxCallerSavedIP) +
                              ((numArgs - (i - 1)) * BytesPerWord))));
  }
  for (i = (numArgs + 1); i <= numStack; i += 1) {
    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(theContext)) && (!(isForwarded(theContext))));
    assert(
        validStorePointerUncheckedArgs(ReceiverIndex + i, theContext, nilObj));
    longAtput((void *)((theContext + BaseHeaderSize) +
                       ((((usqInt)((ReceiverIndex + i)) << (shiftForWord()))))),
              nilObj);
  }

  /* begin setFrameContext:to: */
  longAtput(theFP + FoxThisContext, theContext);
  byteAtput((theFP + FoxFrameFlags) + 2, 1);
  assert(frameHasContext(theFP));
  assert((frameOfMarriedContext(theContext)) == theFP);
  assert((numStack + ReceiverIndex) < (lengthOf(theContext)));
  return theContext;
}