/* Extracted from interp.c:30665 (function
 * allOldMarkedWeakObjectsOnWeaklingStack). */

/* SpurMemoryManager>>#allOldMarkedWeakObjectsOnWeaklingStack */

static sqInt allOldMarkedWeakObjectsOnWeaklingStack(void) {
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt objOop;
  sqInt prevObj;
  sqInt prevPrevObj;

  /* begin allOldSpaceEntitiesDo: */
  /* begin allOldSpaceEntitiesFrom:do: */
  assert(isOldObject(nilObj));
  prevPrevObj = (prevObj = null);
  objOop = nilObj;
  while (1) {
    assert((objOop % (allocationUnit())) == 0);
    if (!(oopisLessThan(objOop, endOfMemory)))
      break;
    assert((long64At((void *)(objOop))) != 0);
    if ((isWeakNonImm(objOop)) &&
        (((byteAt((void *)(objOop + (markBitsByteOffset())))) &
          (1U << (markedBitByteShift()))) != 0)) {
      if (!(isonObjStack(objOop, weaklingStack))) {
        return 0;
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
  return 1;
}