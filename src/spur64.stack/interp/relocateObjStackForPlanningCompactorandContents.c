/* Extracted from interp.c:44988 (function
 * relocateObjStackForPlanningCompactorandContents). */

/*	Relocate all objStack pages that comprise objStack, including contents
   if requested.
 */

/* SpurMemoryManager>>#relocateObjStackForPlanningCompactor:andContents: */

static NoDbgRegParms sqInt relocateObjStackForPlanningCompactorandContents(
    sqInt objStack, sqInt relocateContents) {
  sqInt finishIndex;
  sqInt freeList;
  sqInt fwd;
  sqInt i;
  sqInt next;
  sqInt oop;
  sqInt relocated;
  sqInt result;
  sqInt stackOrNil;

  next = 0;
  if (objStack == nilObj) {
    return objStack;
  }
  assert((relocateContents ? (rawHashBitsOf(objStack)) < ObjStackLimit
                           : (rawHashBitsOf(objStack)) == 0));
  stackOrNil = objStack;
  freeList =
      longAt((void *)((objStack + BaseHeaderSize) +
                      ((((usqInt)(ObjStackFreex) << (shiftForWord()))))));
  while (1) {
    assert((numSlotsOfAny(stackOrNil)) == ObjStackPageSlots);

    /* There are four fixed slots in an obj stack, and a Topx of 0 indicates
       empty, so if there were 5 slots in an oop stack, full would be 2, and the
       last 0-rel index is 4. Hence the last index is topx + fixed slots - 1, or
       topx + ObjStackNextx. The first two slots, Topx and Myx are not object
       references. */
    next = longAt((void *)((stackOrNil + BaseHeaderSize) +
                           ((((usqInt)(ObjStackNextx) << (shiftForWord()))))));
    finishIndex = ObjStackNextx +
                  ((relocateContents ? (long32At((void *)(stackOrNil + 4))) &
                                           (identityHashHalfWordMask())
                                     : 0));

    /* begin relocateObjectsInHeapEntity:from:to: */
    for (i = ObjStackFreex; i <= finishIndex; i += 1) {
      oop = longAt((void *)((stackOrNil + BaseHeaderSize) +
                            ((((usqInt)(i) << (shiftForWord()))))));
      if (((!(oop & (tagMask())))) &&
          (/* isMobile: */
           (oopisGreaterThanOrEqualToandLessThanOrEqualTo(oop, mobileStart,
                                                          lastMobileObject)) &&
           (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) &
               (1U << (pinnedBitByteShift()))) != 0)))) {
        assert(isMarked(oop));
        fwd =
            longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
        assert(isPostMobile(fwd));

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(stackOrNil)) && (!(isForwarded(stackOrNil))));
        assert(validStorePointerUncheckedArgs(i, stackOrNil, fwd));
        longAtput((void *)((stackOrNil + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  fwd);
      }
    }
    relocated = (/* isMobile: */
                 (oopisGreaterThanOrEqualToandLessThanOrEqualTo(
                     stackOrNil, mobileStart, lastMobileObject)) &&
                         (!(((byteAt((void *)(stackOrNil +
                                              (formatFieldByteOffset())))) &
                             (1U << (pinnedBitByteShift()))) != 0))
                     ? longAt((void *)((stackOrNil + BaseHeaderSize) +
                                       (0U << (shiftForWord()))))
                     : stackOrNil);
    if (stackOrNil == objStack) {
      result = relocated;
    }
    if (!(next != 0))
      break;
    stackOrNil = next;
  }
  while (freeList != 0) {
    assert((numSlotsOfAny(freeList)) == ObjStackPageSlots);
    next = longAt((void *)((freeList + BaseHeaderSize) +
                           ((((usqInt)(ObjStackFreex) << (shiftForWord()))))));

    /* begin relocateObjectsInHeapEntity:from:to: */
    for (i = ObjStackFreex; i <= ObjStackFreex; i += 1) {
      oop = longAt((void *)((freeList + BaseHeaderSize) +
                            ((((usqInt)(i) << (shiftForWord()))))));
      if (((!(oop & (tagMask())))) &&
          (/* isMobile: */
           (oopisGreaterThanOrEqualToandLessThanOrEqualTo(oop, mobileStart,
                                                          lastMobileObject)) &&
           (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) &
               (1U << (pinnedBitByteShift()))) != 0)))) {
        assert(isMarked(oop));
        fwd =
            longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
        assert(isPostMobile(fwd));

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(freeList)) && (!(isForwarded(freeList))));
        assert(validStorePointerUncheckedArgs(i, freeList, fwd));
        longAtput((void *)((freeList + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  fwd);
      }
    }
    if (/* isMobile: */
        (oopisGreaterThanOrEqualToandLessThanOrEqualTo(freeList, mobileStart,
                                                       lastMobileObject)) &&
        (!(((byteAt((void *)(freeList + (formatFieldByteOffset())))) &
            (1U << (pinnedBitByteShift()))) != 0))) {
      /* fetchPointer:ofObject: */
      longAt((void *)((freeList + BaseHeaderSize) + (0U << (shiftForWord()))));
    } else {
    }
    freeList = next;
  }
  return relocated;
}