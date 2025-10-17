/* Extracted from interp.c:37004 (function instantiateClass). */

/* SpurMemoryManager>>#instantiateClass: */

static sqInt instantiateClass(sqInt classObj) {
  sqInt classFormat;
  sqInt classIndex;
  sqInt err;
  sqInt hash;
  sqInt instSpec;
  sqInt newObj;
  usqInt newObjUsqInt;
  usqInt numBytes;
  sqInt numSlots;
  usqInt p;
  usqInt toDoLimit;

  classFormat = ((longAt((void *)((classObj + BaseHeaderSize) +
                                  ((((usqInt)(InstanceSpecificationIndex)
                                     << (shiftForWord()))))))) >>
                 3);
  instSpec =
      (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
  if (!(/* isFixedSizePointerFormat: */
        (instSpec <= (nonIndexablePointerFormat())) ||
        (instSpec == (ephemeronFormat())))) {
    return null;
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
    return null;
  }
  numSlots = classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);

  /* begin allocateSlots:format:classIndex: */
  if (numSlots >= (numSlotsMask())) {
    if ((((usqInt)(numSlots)) >> 56) > 0) {
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
    newObj = allocateSlotsInOldSpacebytesformatclassIndex(numSlots, numBytes,
                                                          instSpec, classIndex);
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

  /* for header parsing we put a saturated slot count in the prepended overflow
   * size word */
  assert((numBytes % (allocationUnit())) == 0);
  assert((newObjUsqInt % (allocationUnit())) == 0);
  freeStart += numBytes;
  newObj = newObjUsqInt;
  /* end allocateSlots:format:classIndex: */
l1:
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
  return newObj;
}