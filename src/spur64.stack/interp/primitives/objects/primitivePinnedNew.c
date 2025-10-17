/* Extracted from interp.c:20877 (function primitivePinnedNew). */

/* InterpreterPrimitives>>#primitivePinnedNew */

static void primitivePinnedNew(void) {
  sqInt classFormat;
  sqInt classIndex;
  sqInt classObj;
  sqInt err;
  sqInt hash;
  sqInt instSpec;
  sqInt newObj;
  sqInt numSlots;
  sqInt obj;
  usqInt p;
  sqInt reasonCode;
  char *sp;
  usqInt toDoLimit;

  /* For the mirror prims check that the class obj is actually a valid class. */
  classObj = longAt(stackPointer);

  /* begin inOldSpaceInstantiatePinnedClass: */
  classFormat = ((fetchPointerofObject(InstanceSpecificationIndex, classObj)) >>
                 3);
  instSpec =
      (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
  if (!(/* isFixedSizePointerFormat: */
        (instSpec <= (nonIndexablePointerFormat())) ||
        (instSpec == (ephemeronFormat())))) {
    obj = null;
    goto l1;
  }

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
    goto l1;
  }
  numSlots = classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
  newObj = allocateSlotsForPinningInOldSpacebytesformatclassIndex(
      numSlots, /* objectBytesForSlots: */
      (numSlots ? ((((usqInt)(numSlots) << (shiftForWord())))) +
                      ((numSlots >= (numSlotsMask())
                            ? BaseHeaderSize + BaseHeaderSize
                            : BaseHeaderSize))
                : 8 /* allocationUnit */ + BaseHeaderSize),
      instSpec, classIndex);
  if (newObj) {
    /* begin fillObj:numSlots:with: */
    assert(oopisLessThan(
        ((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1,
        addressAfter(newObj)));
    toDoLimit =
        ((usqInt)(((newObj + BaseHeaderSize) + (numSlots * BytesPerOop)) - 1));
    for (p = (((usqInt)(newObj + BaseHeaderSize))); p <= toDoLimit;
         p += 8 /* allocationUnit */) {
      longAtput((void *)(p), nilObj);
    }
  }
  obj = newObj;
  /* end inOldSpaceInstantiatePinnedClass: */
l1:
  if (obj) {
    /* begin pop:thenPush: */
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              obj);
    stackPointer = sp;
  } else {
    reasonCode =
        (isFixedSizePointerFormat(
             (((usqInt)((
                  ((fetchPointerofObject(InstanceSpecificationIndex, longAt(stackPointer))) >>
                   3)))) >>
              (fixedFieldsFieldWidth())) &
             (formatMask()))
             ? PrimErrNoMemory
             : PrimErrBadReceiver);

    /* begin primitiveFailFor: */
    primFailCode = reasonCode;
  }
}