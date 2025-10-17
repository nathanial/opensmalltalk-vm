/* Extracted from interp.c:48003 (function
 * scanForFirstFreeAndFirstMobileObjectFrom). */

/*	Scan from initialObject, setting firstMobileObject to the first marked
        object after the first free object found, or endOfMemory if none is
   found. Answer the first free object found, or nil if none. */

/* SpurPlanningCompactor>>#scanForFirstFreeAndFirstMobileObjectFrom: */

static sqInt
scanForFirstFreeAndFirstMobileObjectFrom(sqInt initialObject) {
  sqInt firstFree;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt objOop;
  sqInt prevObj;
  sqInt prevPrevObj;

  firstFree = 0;
  firstMobileObject = endOfMemory;

  /* begin allOldSpaceEntitiesFrom:do: */
  assert(isOldObject(initialObject));
  prevPrevObj = (prevObj = null);
  objOop = initialObject;
  while (1) {
    assert((objOop % (allocationUnit())) == 0);
    if (!(oopisLessThan(objOop, endOfMemory)))
      break;
    assert((long64At((void *)(objOop))) != 0);
    if ((byteAt((void *)(objOop + (markBitsByteOffset())))) &
        (1U << (markedBitByteShift()))) {
      if (firstFree) {
        firstMobileObject = objOop;
        return firstFree;
      }
    } else {
      if (!firstFree) {
        firstFree = objOop;
      }
    }
    prevPrevObj = prevObj;
    prevObj = objOop;

    /* begin objectAfter:limit: */
    followingWordAddress = addressAfter(objOop);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objOop = endOfMemory;
      goto l1;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                      (numSlotsMask())
                  ? followingWordAddress + BaseHeaderSize
                  : followingWordAddress);
    /* end objectAfter:limit: */
  l1:;
  }
  return firstFree;
}