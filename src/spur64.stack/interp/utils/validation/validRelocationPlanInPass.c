/* Extracted from interp.c:48886 (function validRelocationPlanInPass). */

/*	Answer 0 if all the mobile objects from firstMobileObject to
        lastMobileObject have sane forwarding addresses, and that
        savedFirstFieldsSpace is of
        matching capacity. Otherwise answer an error code identifying the
   anomaly. */

/* SpurPlanningCompactor>>#validRelocationPlanInPass: */

static sqInt validRelocationPlanInPass(sqInt onePass) {
  usqInt destination;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt nMobiles;
  sqInt objOop;
  sqInt prevObj;
  sqInt prevPrevObj;
  usqInt toFinger;

  nMobiles = 0;
  toFinger = mobileStart;
  anomaly = null;

  /* begin allOldSpaceEntitiesFrom:do: */
  assert(isOldObject(firstMobileObject));
  prevPrevObj = (prevObj = null);
  objOop = firstMobileObject;
  while (1) {
    assert((objOop % (allocationUnit())) == 0);
    if (!(oopisLessThan(objOop, endOfMemory)))
      break;
    assert((long64At((void *)(objOop))) != 0);
    if ((byteAt((void *)(objOop + (markBitsByteOffset())))) &
        (1U << (markedBitByteShift()))) {
      if (!((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
            (1U << (pinnedBitByteShift())))) {
        nMobiles += 1;
        destination = fetchPointerofObject(0U, objOop);
        if (!(destination >= toFinger)) {
          anomaly = objOop;
          return 1;
        }
        toFinger += bytesInBody(objOop);
        if (oopisGreaterThan(objOop, lastMobileObject)) {
          anomaly = objOop;
          return 2;
        }
        if (objOop == lastMobileObject) {
          return ((((((savedFirstFieldsSpace.top)) + BytesPerOop) -
                    ((savedFirstFieldsSpace.start))) /
                   BytesPerOop) == nMobiles
                      ? 0
                      : 3);
        }
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

  /* N.B. written this way so that if there are no mobiles the expression
   * evaluates to 0 in Smalltalk /and/ in C unsigned arithmetic. */
  return ((((((savedFirstFieldsSpace.top)) + BytesPerOop) -
            ((savedFirstFieldsSpace.start))) /
           BytesPerOop) == nMobiles
              ? 0
              : 4);
}