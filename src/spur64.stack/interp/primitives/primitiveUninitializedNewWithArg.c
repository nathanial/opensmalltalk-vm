/* Extracted from interp.c:25424 (function primitiveUninitializedNewWithArg). */

/*	Allocate a new indexable instance. Fail if the allocation would leave
   less than lowSpaceThreshold bytes free. May cause a GC.
 */

/* InterpreterPrimitives>>#primitiveUninitializedNewWithArg */

static void primitiveUninitializedNewWithArg(void) {
  sqInt classFormat;
  sqInt classIndex;
  sqInt classObj;
  sqInt err;
  sqInt hash;
  sqInt instSpec;
  sqInt instSpecSqInt;
  sqInt newObj;
  usqInt newObjUsqInt;
  usqInt numBytes;
  usqInt numSlots;
  sqInt obj;
  sqInt reasonCode;
  usqIntptr_t size;
  char *sp;
  sqInt value;

  /* For the mirror prims check that the class obj is actually a valid class. */

  /* begin positiveMachineIntegerValueOf: */
  if (((((longAt(stackPointer))) & 7) == 1)) {
    value = ((longAt(stackPointer)) >> 3);
    if (value < 0) {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
      size = null;
      goto l1;
    }
    size = value;
    goto l1;
  }

  /* don't inline the rare case */
  size = positiveMachineIntegerValueOfObj(longAt(stackPointer));
  /* end positiveMachineIntegerValueOf: */
l1:
  if (primFailCode) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }

  /* positiveMachineIntegerValueOf: succeeds only for non-negative integers. */
  classObj = longAt(stackPointer + (1 * BytesPerWord));

  /* begin instantiateUninitializedClass:indexableSize: */
  classFormat =
      ((fetchPointerofObject(InstanceSpecificationIndex, classObj)) >> 3);
  instSpecSqInt =
      (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
  classIndex =
      (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask());
  assert(isPureBitsFormat(instSpecSqInt));
  switch (instSpecSqInt) {
  case sixtyFourBitIndexableFormat():
    numSlots = size;
    break;
  case firstLongFormat():
    if ((classIndex == ClassFloatCompactIndex) && (size != 2)) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadReceiver;
      obj = null;
      goto l3;
    }
    numSlots = (size + 1) / 2;
    instSpecSqInt += size & 1;
    break;
  case firstShortFormat():
    numSlots = (size + 3) / 4;
    instSpecSqInt += (4 - size) & 3;
    break;
  case firstByteFormat():
    numSlots = (size + 7) / 8;
    instSpecSqInt += (8 - size) & 7;
    break;
  default:
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    obj = null;
    goto l3;
  }

  /* not bits indexable */
  if (!classIndex) {
    /* begin ensureBehaviorHash: */
    assert(addressCouldBeClassObj(classObj));

    /* eem 12/28/2021 the above asserft is too weak (and only an assert) */
    classIndex = ((hash = (long32At((void *)(classObj + 4))) &
                          (identityHashHalfWordMask()))
                      ? hash
                      : (objCouldBeClassObj(classObj)
                             ? ((err = enterIntoClassTable(classObj))
                                    ? -err
                                    : (long32At((void *)(classObj + 4))) &
                                          (identityHashHalfWordMask()))
                             : -PrimErrBadReceiver));
    if (classIndex < 0) {
      /* primitiveFailFor: */
      primFailCode = -classIndex;
      obj = null;
      goto l3;
    }
  }
  if (numSlots > ((1U << (fixedFieldsFieldWidth())) - 1)) {
    if (numSlots > (0x10000000000LL)) {
      /* primitiveFailFor: */
      primFailCode = PrimErrUnsupported;
      obj = null;
      goto l3;
    }
    newObj = allocateSlotsInOldSpacebytesformatclassIndex(
        numSlots, /* objectBytesForSlots: */
        (numSlots ? ((numSlots << (shiftForWord()))) +
                        ((numSlots >= (numSlotsMask())
                              ? BaseHeaderSize + BaseHeaderSize
                              : BaseHeaderSize))
                  : 8 /* allocationUnit */ + BaseHeaderSize),
        instSpecSqInt, classIndex);
  } else {
    /* begin allocateSlots:format:classIndex: */
    if (numSlots >= (numSlotsMask())) {
      if (((numSlots) >> 56) > 0) {
        newObj = null;
        goto l2;
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
          numSlots, numBytes, instSpecSqInt, classIndex);
      goto l2;
    }
    if (numSlots >= (numSlotsMask())) {
      longAtput((void *)(freeStart),
                ((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) +
                    numSlots);
      longAtput((void *)(newObjUsqInt),
                ((((((usqLong)(numSlotsMask()))) << (numSlotsFullShift()))) +
                 ((((usqInt)(instSpecSqInt) << (formatShift()))))) +
                    classIndex);
    } else {
      longAtput((void *)(newObjUsqInt),
                ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
                 ((((usqInt)(instSpecSqInt) << (formatShift()))))) +
                    classIndex);
    }

    /* for header parsing we put a saturated slot count in the prepended
     * overflow size word */
    assert((numBytes % (allocationUnit())) == 0);
    assert((newObjUsqInt % (allocationUnit())) == 0);
    freeStart += numBytes;
    newObj = newObjUsqInt;
    /* end allocateSlots:format:classIndex: */
  l2:;
  }
  obj = newObj;
  /* end instantiateUninitializedClass:indexableSize: */
l3:
  if (obj) {
    /* begin pop:thenPush: */
    popthenPush(argumentCount + 1, obj);
  } else {
    instSpec = (((usqInt)((((fetchPointerofObject(
                                InstanceSpecificationIndex,
                                longAt(stackPointer + (1 * BytesPerWord)))) >>
                            3)))) >>
                (fixedFieldsFieldWidth())) &
               (formatMask());
    reasonCode = ((/* isIndexableFormat: */
                   (instSpec >= (arrayFormat())) &&
                   ((instSpec <= (weakArrayFormat())) ||
                    (instSpec >= (sixtyFourBitIndexableFormat())))) &&
                          (!(instSpec >= (firstCompiledMethodFormat())))
                      ? PrimErrNoMemory
                      : PrimErrBadReceiver);

    /* begin primitiveFailFor: */
    primFailCode = reasonCode;
  }
}