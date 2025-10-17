/* Extracted from interp.c:29524 (function accessibleObjectAfter). */

/*	Answer the accessible object following the given object or
        free chunk in the heap. Return nil when heap is exhausted.
        This is for primitiveNextObject subsequent to primitiveSomeObject.
        It also tries to handle more general use by ordering objects as
        eden
        past
        old
        but this is tricky becaus ethe order in memory is
        past
        eden
        old */

/* SpurMemoryManager>>#accessibleObjectAfter: */

static sqInt accessibleObjectAfter(sqInt objOop) {
  sqInt address;
  sqInt followingWord;
  usqInt followingWordAddress;
  usqInt numSlots;
  sqInt objAfter;

  objAfter = objOop;
  if (oopisLessThan(objAfter, nilObj)) {
    assert((isInEden(objOop)) || (isInPastSpace(objOop)));
    if (oopisGreaterThan(objAfter, pastSpaceStart)) {
      while (1) {
        /* begin objectAfter:limit: */
        followingWordAddress = addressAfter(objAfter);
        if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
          objAfter = freeStart;
          goto l1;
        }
        followingWord = longAt((void *)(followingWordAddress));
        objAfter = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                            (numSlotsMask())
                        ? followingWordAddress + BaseHeaderSize
                        : followingWordAddress);
        /* end objectAfter:limit: */
      l1:;
        if (!(oopisLessThan(objAfter, freeStart)))
          break;
        if (((longAt((void *)(objAfter))) & (classIndexMask())) >
            (lastClassIndexPun())) {
          return objAfter;
        }
      }

      /* There wasn't a next object in eden. If past space is empty answer
       * nilObj. */
      if (pastSpaceStart <= (((pastSpace).start))) {
        return nilObj;
      }

      /* If the first object in pastSpace is OK, answer it, otherwise fall
       * through to enumerate past space. */
      address = ((pastSpace).start);

      /* begin objectStartingAt: */
      numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
      objAfter =
          (numSlots == (numSlotsMask()) ? address + BaseHeaderSize : address);
      if (((longAt((void *)(objAfter))) & (classIndexMask())) >
          (lastClassIndexPun())) {
        return objAfter;
      }
    }

    /* Obj is in eden.  Answer next normal object in eden, if there is one.
       Either objOop was in pastSpace, or enumeration exhaused eden, so
       enumerate past space. */
    while (1) {
      /* begin objectAfter:limit: */
      followingWordAddress = addressAfter(objAfter);
      if (oopisGreaterThanOrEqualTo(followingWordAddress, pastSpaceStart)) {
        objAfter = pastSpaceStart;
        goto l2;
      }
      followingWord = longAt((void *)(followingWordAddress));
      objAfter = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                          (numSlotsMask())
                      ? followingWordAddress + BaseHeaderSize
                      : followingWordAddress);
      /* end objectAfter:limit: */
    l2:;
      if (!(oopisLessThan(objAfter, pastSpaceStart)))
        break;
      if (((longAt((void *)(objAfter))) & (classIndexMask())) >
          (lastClassIndexPun())) {
        return objAfter;
      }
    }
    return nilObj;
  }

  /* object in new space */
  while (1) {
    /* begin objectAfter:limit: */
    followingWordAddress = addressAfter(objAfter);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objAfter = endOfMemory;
      goto l3;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objAfter = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                        (numSlotsMask())
                    ? followingWordAddress + BaseHeaderSize
                    : followingWordAddress);
    /* end objectAfter:limit: */
  l3:
    if (objAfter == endOfMemory) {
      return null;
    }
    if (((longAt((void *)(objAfter))) & (classIndexMask())) >
        (lastClassIndexPun())) {
      return objAfter;
    }
  }
  return 0;
}