/* Extracted from interp.c:47498 (function copyAndUnmark). */

/*	Sweep the heap, unmarking all objects and moving mobile objects to their
        correct positions,
        restoring their savedFirstFields. */

/* SpurPlanningCompactor>>#copyAndUnmark: */

static NeverInline void copyAndUnmark(sqInt firstPass) {
  sqInt classIndex;
  sqInt finalPass;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt objOopSqInt;
  sqInt prevObj;
  sqInt prevPrevObj;

  if (firstPass) {
    /* begin unmarkInitialImmobileObjects */
    /* begin allOldSpaceObjectsFrom:do: */
    /* begin allOldSpaceEntitiesFrom:do: */
    assert(isOldObject(nilObj));
    prevPrevObj = (prevObj = null);
    objOopSqInt = nilObj;
    while (1) {
      assert((objOopSqInt % (allocationUnit())) == 0);
      if (!(oopisLessThan(objOopSqInt, endOfMemory)))
        break;
      assert((long64At((void *)(objOopSqInt))) != 0);

      /* begin isEnumerableObject: */
      classIndex = (longAt((void *)(objOopSqInt))) & (classIndexMask());
      assert((classIndex == (segmentBridgePun())) ||
             ((classIndex == (isForwardedObjectClassIndexPun())) ||
              (((long64At((void *)(objOopSqInt))) != 0) &&
               (classIndex < (numClassTablePages * (classTablePageSize()))))));
      if (classIndex >= (isForwardedObjectClassIndexPun())) {
        if (oopisGreaterThanOrEqualTo(objOopSqInt, firstMobileObject)) {
          goto l2;
        }

        /* begin setIsMarkedOf:to: */
        assert(!(isFreeObject(objOopSqInt)));
        byteAtput((void *)(objOopSqInt + (markBitsByteOffset())),
                  (byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) &
                      (0xFF - (1U << (markedBitByteShift()))));
      }
      prevPrevObj = prevObj;
      prevObj = objOopSqInt;

      /* begin objectAfter:limit: */
      followingWordAddress = addressAfter(objOopSqInt);
      if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
        objOopSqInt = endOfMemory;
        goto l1;
      }
      followingWord = longAt((void *)(followingWordAddress));
      objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                             (numSlotsMask())
                         ? followingWordAddress + BaseHeaderSize
                         : followingWordAddress);
      /* end objectAfter:limit: */
    l1:;
    }
    /* end unmarkInitialImmobileObjects */
  l2:;
  }
  finalPass = copyAndUnmarkMobileObjects();
  if ((lastMobileObject) && ((!finalPass) && (biasForGC))) {
    unmarkObjectsFromFirstFreeObject();
  }
}