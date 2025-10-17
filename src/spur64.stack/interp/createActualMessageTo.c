/* Extracted from interp.c:51611 (function createActualMessageTo). */

/*	Bundle up the selector, arguments and lookupClass into a Message object.
        In the process it pops the arguments off the stack, and pushes the
   message object. This can then be presented as the argument of e.g.
   #doesNotUnderstand: */

/* StackInterpreter>>#createActualMessageTo: */

static NoDbgRegParms void createActualMessageTo(sqInt lookupClass) {
  usqInt argumentArray;
  sqInt i;
  usqInt message;
  usqInt newObj;
  usqInt numBytes;
  char *sp;

  /* This is a useful break-point */
  assert((isImmediate(messageSelector)) ||
         (addressCouldBeObj(messageSelector)));

  /* begin mnuBreakpoint:receiver: */
  mnuBreakpointreceiver(firstFixedFieldOfMaybeImmediate(messageSelector),
                        lengthOfMaybeImmediate(messageSelector), null);
  assert((argumentCount >= 0) &&
         ((knownClassAtIndex(ClassArrayCompactIndex)) != nilObj));
  assert((arrayFormat()) ==
         (instSpecOfClass(knownClassAtIndex(ClassArrayCompactIndex))));

  /* begin allocateSmallNewSpaceSlots:format:classIndex: */
  assert(argumentCount < (numSlotsMask()));
  newObj = freeStart;
  numBytes =
      BaseHeaderSize + ((argumentCount < 1 ? 8 /* allocationUnit */
                                           : argumentCount * BytesPerOop));
  assert((numBytes % (allocationUnit())) == 0);
  assert((newObj % (allocationUnit())) == 0);
  if ((freeStart + numBytes) > scavengeThreshold) {
    if (!needGCFlag) {
      /* begin scheduleScavenge */
      needGCFlag = 1;
      forceInterruptCheck();
    }
    if ((freeStart + numBytes) > (((eden).limit))) {
      error(
          "no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
      argumentArray = 0;
      goto l1;
    }
  }
  long64Atput((void *)(newObj),
              ((((((usqLong)argumentCount)) << (numSlotsFullShift()))) +
               ((((usqInt)((arrayFormat())) << (formatShift()))))) +
                  ClassArrayCompactIndex);
  freeStart += numBytes;
  argumentArray = newObj;
  /* end eeInstantiateSmallClassIndex:format:numSlots: */
l1:

  /* begin eeInstantiateSmallClassIndex:format:numSlots: */
  assert(((MessageLookupClassIndex + 1) >= 0) &&
         ((knownClassAtIndex(ClassMessageCompactIndex)) != nilObj));
  assert((nonIndexablePointerFormat()) ==
         (instSpecOfClass(knownClassAtIndex(ClassMessageCompactIndex))));

  /* begin allocateSmallNewSpaceSlots:format:classIndex: */
  assert((MessageLookupClassIndex + 1) < (numSlotsMask()));
  newObj = freeStart;
  numBytes =
      BaseHeaderSize + (((MessageLookupClassIndex + 1) < 1
                             ? 8 /* allocationUnit */
                             : (MessageLookupClassIndex + 1) * BytesPerOop));
  assert((numBytes % (allocationUnit())) == 0);
  assert((newObj % (allocationUnit())) == 0);
  if ((freeStart + numBytes) > scavengeThreshold) {
    if (!needGCFlag) {
      /* begin scheduleScavenge */
      needGCFlag = 1;
      forceInterruptCheck();
    }
    if ((freeStart + numBytes) > (((eden).limit))) {
      error(
          "no room in eden for allocateSmallNewSpaceSlots:format:classIndex:");
      message = 0;
      goto l2;
    }
  }
  long64Atput(
      (void *)(newObj),
      ((((((usqLong)(MessageLookupClassIndex + 1))) << (numSlotsFullShift()))) +
       ((((usqInt)((nonIndexablePointerFormat())) << (formatShift()))))) +
          ClassMessageCompactIndex);
  freeStart += numBytes;
  message = newObj;
  /* end eeInstantiateSmallClassIndex:format:numSlots: */
l2:

  /* Since the array is new can use unchecked stores. */
  for (i = ((argumentCount - 1) * BytesPerOop); i >= 0; i += (-BytesPerOop)) {
    longAtput((void *)((argumentArray + BaseHeaderSize) + i), popStack());
  }

  /* Since message is new can use unchecked stores. */

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(message)) && (!(isForwarded(message))));
  assert(validStorePointerUncheckedArgs(MessageSelectorIndex, message,
                                        messageSelector));
  longAtput((void *)((message + BaseHeaderSize) +
                     ((((usqInt)(MessageSelectorIndex) << (shiftForWord()))))),
            messageSelector);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(message)) && (!(isForwarded(message))));
  assert(validStorePointerUncheckedArgs(MessageArgumentsIndex, message,
                                        argumentArray));
  longAtput((void *)((message + BaseHeaderSize) +
                     ((((usqInt)(MessageArgumentsIndex) << (shiftForWord()))))),
            argumentArray);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(message)) && (!(isForwarded(message))));
  assert(validStorePointerUncheckedArgs(MessageLookupClassIndex, message,
                                        lookupClass));
  longAtput(
      (void *)((message + BaseHeaderSize) +
               ((((usqInt)(MessageLookupClassIndex) << (shiftForWord()))))),
      lookupClass);

  /* begin push: */
  longAtput((sp = stackPointer - BytesPerWord), message);
  stackPointer = sp;
  argumentCount = 1;
}