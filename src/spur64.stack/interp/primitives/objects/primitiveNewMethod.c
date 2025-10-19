/* Extracted from interp.c:19459 (function primitiveNewMethod). */

/* InterpreterPrimitives>>#primitiveNewMethod */

static void primitiveNewMethod(void) {
  sqInt bytecodeCount;
  sqInt class;
  sqInt classFormat;
  sqInt classIndex;
  sqInt err;
  sqInt hash;
  sqInt header;
  sqInt i;
  sqInt instSpec;
  usqInt literalCount;
  sqInt newObj;
  usqInt newObjUsqInt;
  usqInt numBytes;
  usqInt numSlots;
  usqInt p;
  sqInt reasonCode;
  sqInt size;
  sqInt theMethod;
  usqInt toDoLimit;

  header = longAt(stackPointer);
  bytecodeCount = longAt(stackPointer + (1 * BytesPerWord));
  if (!(((((header) & 7) == 1)) &&
        (((((bytecodeCount) & 7) == 1)) &&
         (((bytecodeCount = (bytecodeCount >> 3))) >= 0)))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  class = longAt(stackPointer + (2 * BytesPerWord));

  /* begin literalCountOfMethodHeader: */
  assert((((header) & 7) == 1));
  literalCount = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
  size = ((literalCount + LiteralStart) * BytesPerOop) + bytecodeCount;

  /* begin instantiateCompiledMethodClass:indexableSize: */
  classFormat =
      ((fetchPointerofObject(InstanceSpecificationIndex, class)) >> 3);
  instSpec =
      (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
  if (instSpec != (firstCompiledMethodFormat())) {
    theMethod = null;
    goto l2;
  }
  numSlots = (size + 7) / 8;
  instSpec += (8 - size) & 7;

  /* begin ensureBehaviorHash: */
  assert(addressCouldBeClassObj(class));

  /* eem 12/28/2021 the above asserft is too weak (and only an assert) */
  classIndex =
      ((hash = (long32At((void *)(class + 4))) & (identityHashHalfWordMask()))
           ? hash
           : (objCouldBeClassObj(class)
                  ? ((err = enterIntoClassTable(class))
                         ? -err
                         : (long32At((void *)(class + 4))) &
                               (identityHashHalfWordMask()))
                  : -PrimErrBadReceiver));
  if (classIndex < 0) {
    /* primitiveFailFor: */
    primFailCode = -classIndex;
    theMethod = null;
    goto l2;
  }
  if (numSlots > ((1U << (fixedFieldsFieldWidth())) - 1)) {
    if (numSlots > (0x10000000000LL)) {
      theMethod = null;
      goto l2;
    }
    newObj = allocateSlotsInOldSpacebytesformatclassIndex(
        numSlots, /* objectBytesForSlots: */
        (numSlots ? ((numSlots << (shiftForWord()))) +
                        ((numSlots >= (numSlotsMask())
                              ? BaseHeaderSize + BaseHeaderSize
                              : BaseHeaderSize))
                  : 8 /* allocationUnit */ + BaseHeaderSize),
        instSpec, classIndex);
  } else {
    /* begin allocateSlots:format:classIndex: */
    if (numSlots >= (numSlotsMask())) {
      if (((numSlots) >> 56) > 0) {
        newObj = null;
        goto l1;
      }
      newObjUsqInt = freeStart + BaseHeaderSize;
      numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
    } else {
      newObjUsqInt = freeStart;
      numBytes = BaseHeaderSize + ((numSlots < 1 ? 8 /* allocationUnit */
                                                 : numSlots * BytesPerOop));
    }
    if ((freeStart + numBytes) > scavengeThreshold) {
      if (numSlots <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
        if (!needGCFlag) {
          /* begin scheduleScavenge */
          needGCFlag = 1;
          forceInterruptCheck();
        }
      }
      newObj = allocateSlotsInOldSpacebytesformatclassIndex(
          numSlots, numBytes, instSpec, classIndex);
      goto l1;
    }
    if (numSlots >= (numSlotsMask())) {
      longAtput((void *)(freeStart),
                ((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) +
                    numSlots);
      longAtput((void *)(newObjUsqInt),
                ((((((usqLong)(numSlotsMask()))) << (numSlotsFullShift()))) +
                 ((((usqInt)(instSpec) << (formatShift()))))) +
                    classIndex);
    } else {
      longAtput((void *)(newObjUsqInt),
                ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
                 ((((usqInt)(instSpec) << (formatShift()))))) +
                    classIndex);
    }

    /* for header parsing we put a saturated slot count in the prepended
     * overflow size word */
    assert((numBytes % (allocationUnit())) == 0);
    assert((newObjUsqInt % (allocationUnit())) == 0);
    freeStart += numBytes;
    newObj = newObjUsqInt;
    /* end allocateSlots:format:classIndex: */
  l1:;
  }
  if (newObj) {
    /* begin fillObj:numSlots:with: */
    assert(oopisLessThan(
        ((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1,
        addressAfter(newObj)));
    toDoLimit =
        ((usqInt)(((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1));
    for (p = (((usqInt)(newObj + BaseHeaderSize))); p <= toDoLimit;
         p += 8 /* allocationUnit */) {
      longAtput((void *)(p), 0);
    }
  }
  theMethod = newObj;
  /* end instantiateCompiledMethodClass:indexableSize: */
l2:
  if (!theMethod) {
    reasonCode =
        (isCompiledMethodFormat(
             (((usqInt)((
                  ((fetchPointerofObject(InstanceSpecificationIndex, class)) >>
                   3)))) >>
              (fixedFieldsFieldWidth())) &
             (formatMask()))
             ? PrimErrNoMemory
             : PrimErrBadReceiver);

    /* begin primitiveFailFor: */
    primFailCode = reasonCode;
    return;
  }

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(theMethod)) && (!(isForwarded(theMethod))));
  assert(validStorePointerUncheckedArgs(HeaderIndex, theMethod, header));
  longAtput((void *)((theMethod + BaseHeaderSize) +
                     ((((usqInt)(HeaderIndex) << (shiftForWord()))))),
            header);
  for (i = 1; i <= literalCount; i += 1) {
    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(theMethod)) && (!(isForwarded(theMethod))));
    assert(validStorePointerUncheckedArgs(i, theMethod, nilObj));
    longAtput((void *)((theMethod + BaseHeaderSize) +
                       ((((usqInt)(i) << (shiftForWord()))))),
              nilObj);
  }

  /* begin pop:thenPush: */
  popthenPush(3, theMethod);
}