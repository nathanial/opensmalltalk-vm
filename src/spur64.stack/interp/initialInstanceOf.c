/* Extracted from interp.c:36389 (function initialInstanceOf). */

/* SpurMemoryManager>>#initialInstanceOf: */

static NoDbgRegParms sqInt initialInstanceOf(sqInt classObj) {
  sqInt classIndex;
  sqInt classIndexSqInt;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt objOopSqInt;
  sqInt prevObj;
  sqInt prevPrevObj;

  classIndex =
      (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask());
  if (classIndex) {
    /* begin flushNewSpaceInstancesOf: */
    classIndexSqInt =
        (long32At((void *)(classObj + 4))) & (identityHashHalfWordMask());
    if (!classIndexSqInt) {
      goto l1;
    }

    /* no instances; nothing to do */

    /* begin tenuringClassIndex: */
    tenuringClassIndex = classIndexSqInt;
    scavengingGCTenuringIf(TenureByClass);
    assert(!(existInstancesInNewSpaceOf(classObj)));
    /* end flushNewSpaceInstancesOf: */
  l1:

    /* scan old space for the first one found. */

    /* begin allOldSpaceObjectsDo: */
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
      classIndexSqInt = (longAt((void *)(objOopSqInt))) & (classIndexMask());
      assert((classIndexSqInt == (segmentBridgePun())) ||
             ((classIndexSqInt == (isForwardedObjectClassIndexPun())) ||
              (((long64At((void *)(objOopSqInt))) != 0) &&
               (classIndexSqInt <
                (numClassTablePages * (classTablePageSize()))))));
      if (classIndexSqInt >= (isForwardedObjectClassIndexPun())) {
        if (classIndex ==
            ((longAt((void *)(objOopSqInt))) & (classIndexMask()))) {
          return objOopSqInt;
        }
      }
      prevPrevObj = prevObj;
      prevObj = objOopSqInt;

      /* begin objectAfter:limit: */
      followingWordAddress = addressAfter(objOopSqInt);
      if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
        objOopSqInt = endOfMemory;
        goto l2;
      }
      followingWord = longAt((void *)(followingWordAddress));
      objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                             (numSlotsMask())
                         ? followingWordAddress + BaseHeaderSize
                         : followingWordAddress);
      /* end objectAfter:limit: */
    l2:;
    }
  }

  /* flush instances in newSpace to settle the enumeration. */
  return null;
}