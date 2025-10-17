/* Extracted from interp.c:47564 (function endCompaction). */

/* SpurPlanningCompactor>>#endCompaction */

static void endCompaction(void) {
  sqInt address;
  sqInt followingWord;
  usqInt followingWordAddress;
  usqInt numSlots;
  sqInt objOop;
  sqInt objOopSqInt;
  sqInt prevObj;
  sqInt prevPrevObj;

  /* begin unmarkSurvivingObjectsForCompact */
  /* begin allPastSpaceObjectsDo: */
  /* begin allPastSpaceEntitiesDo: */
  prevPrevObj = (prevObj = null);
  address = ((pastSpace).start);

  /* begin objectStartingAt: */
  numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
  objOopSqInt =
      (numSlots == (numSlotsMask()) ? address + BaseHeaderSize : address);
  while (oopisLessThan(objOopSqInt, pastSpaceStart)) {
    assert(isEnumerableObjectNoAssert(objOopSqInt));
    if ((byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) &
        (1U << (markedBitByteShift()))) {
      /* begin setIsMarkedOf:to: */
      assert(!(isFreeObject(objOopSqInt)));
      byteAtput((void *)(objOopSqInt + (markBitsByteOffset())),
                (byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) &
                    (0xFF - (1U << (markedBitByteShift()))));
    }
    prevPrevObj = prevObj;
    prevObj = objOopSqInt;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(objOopSqInt);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, pastSpaceStart)) {
      objOopSqInt = pastSpaceStart;
      goto l1;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOopSqInt =
        ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                 (numSlotsMask())
             ? ((oopisLessThan(objOopSqInt, oldSpaceStart)) &&
                        ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
                    ? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
                    : followingWordAddress + BaseHeaderSize)
             : followingWordAddress);
    /* end objectAfterMaybeSlimBridge:limit: */
  l1:;
  }

  /* begin endSlidingCompaction */
  gcPhaseInProgress = 0;
  if (rememberedSetSize > 0) {
    objOop = fetchPointerofObject(RememberedSetRootIndex, hiddenRootsObj);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(objOop)) && (!(isForwarded(objOop))));
    assert(
        validStorePointerUncheckedArgs(0, objOop, firstFieldOfRememberedSet));
    longAtput((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))),
              firstFieldOfRememberedSet);
  }
  setIsPinnedOfto(fetchPointerofObject(RememberedSetRootIndex, hiddenRootsObj),
                  1);

  /* begin relocateRememberedSet */
  rememberedSet = firstIndexableField(
      fetchPointerofObject(RememberedSetRootIndex, hiddenRootsObj));
  if (/* savedFirstFieldsSpaceWasAllocated */
      savedFirstFieldsSpaceNotInOldSpace &&
      (oopisGreaterThan((savedFirstFieldsSpace.start), nilObj))) {
    sqDeallocateMemorySegmentAtOfSize(((void *)((savedFirstFieldsSpace.start))),
                                      ((savedFirstFieldsSpace.limit)) -
                                          ((savedFirstFieldsSpace.start)));
  }
}