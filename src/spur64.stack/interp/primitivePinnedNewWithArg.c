/* Extracted from interp.c:20964 (function primitivePinnedNewWithArg). */

/*	Allocate a new pinned indexable instance. Fail if the allocation would
        leave less than lowSpaceThreshold bytes free.
 */

/* InterpreterPrimitives>>#primitivePinnedNewWithArg */

static void primitivePinnedNewWithArg(void) {
  sqInt classFormat;
  sqInt classIndex;
  sqInt classObj;
  sqInt err;
  sqInt fillValue;
  sqInt hash;
  sqInt instSpec;
  sqInt instSpecSqInt;
  sqInt newObj;
  usqInt numSlots;
  sqInt obj;
  usqInt p;
  sqInt reasonCode;
  usqIntptr_t size;
  char *sp;
  usqInt toDoLimit;
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

  /* begin inOldSpaceInstantiatePinnedClass:indexableSize: */
  classFormat = ((longAt((void *)((classObj + BaseHeaderSize) +
                                  ((((usqInt)(InstanceSpecificationIndex)
                                     << (shiftForWord()))))))) >>
                 3);
  instSpecSqInt =
      (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
  classIndex =
      (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask());
  fillValue = 0;
  switch (instSpecSqInt) {
  case arrayFormat():
    numSlots = size;
    fillValue = nilObj;
    break;
  case indexablePointersFormat():
  case weakArrayFormat():
    numSlots = (classFormat & ((1U << (fixedFieldsFieldWidth())) - 1)) + size;
    fillValue = nilObj;
    break;
  case sixtyFourBitIndexableFormat():
    numSlots = size;
    break;
  case firstLongFormat():
    if ((classIndex == ClassFloatCompactIndex) && (size != 2)) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadReceiver;
      obj = null;
      goto l2;
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
    if ((size != 0) || (instSpecSqInt > 5 /* lastPointerFormat */)) {
      obj = null;
      goto l2;
    }
    numSlots = classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
    fillValue = nilObj;
  }

  /* non-indexable
     Some Squeak images include funky fixed subclasses of abstract variable
     superclasses. e.g. DirectoryEntry as a subclass of ArrayedCollection.
     The (Threaded)FFIPlugin expects to be able to instantiate ExternalData via
     this method.
     Hence allow fixed classes to be instantiated here iff nElements = 0. */
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
      goto l2;
    }
  }
  if (numSlots > (0x10000000000LL)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrUnsupported;
    obj = null;
    goto l2;
  }
  newObj = allocateSlotsForPinningInOldSpacebytesformatclassIndex(
      numSlots, /* objectBytesForSlots: */
      (numSlots ? ((numSlots << (shiftForWord()))) +
                      ((numSlots >= (numSlotsMask())
                            ? BaseHeaderSize + BaseHeaderSize
                            : BaseHeaderSize))
                : 8 /* allocationUnit */ + BaseHeaderSize),
      instSpecSqInt, classIndex);
  if (newObj) {
    /* begin fillObj:numSlots:with: */
    assert(oopisLessThan(
        ((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1,
        addressAfter(newObj)));
    toDoLimit =
        ((usqInt)(((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1));
    for (p = (((usqInt)(newObj + BaseHeaderSize))); p <= toDoLimit;
         p += 8 /* allocationUnit */) {
      longAtput((void *)(p), fillValue);
    }
  }
  obj = newObj;
  /* end inOldSpaceInstantiatePinnedClass:indexableSize: */
l2:
  if (obj) {
    /* begin pop:thenPush: */
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              obj);
    stackPointer = sp;
  } else {
    instSpec =
        (((usqInt)((
             ((longAt((void *)(((longAt(stackPointer + (1 * BytesPerWord))) +
                                BaseHeaderSize) +
                               ((((usqInt)(InstanceSpecificationIndex)
                                  << (shiftForWord()))))))) >>
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