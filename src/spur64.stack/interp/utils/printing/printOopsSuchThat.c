/* Extracted from interp.c:44369 (function printOopsSuchThat). */

/*	useful for VM debugging */

/* SpurMemoryManager>>#printOopsSuchThat: */

NeverInline void printOopsSuchThat(sqInt (*function)(sqInt)) {
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt n;
  usqInt numSlots;
  sqInt objOop;
  sqInt prevObj;
  sqInt prevPrevObj;
  usqInt start;

  n = 0;

  /* begin allHeapEntitiesDo: */
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
    if (function(objOop)) {
      n += 1;
      printEntity(objOop);
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

  /* begin allNewSpaceEntitiesDo: */
  prevPrevObj = (prevObj = null);

  /* After a scavenge eden is empty, futureSpace is empty, and all newSpace
     objects are in pastSpace.  Objects are allocated in eden.  So enumerate
     only pastSpace and eden. */
  assert((((pastSpace).start)) < (((eden).start)));
  start = /* startAddressForBridgedHeapEnumeration */
      (pastSpaceStart > (((pastSpace).start))
           ? ((pastSpace).start)
           : (freeStart > (((eden).start)) ? ((eden).start) : oldSpaceStart));
  if (start > freeStart) {
    goto l3;
  }

  /* begin bridgePastSpaceAndEden */
  if (pastSpaceStart < (((eden).start))) {
    if ((pastSpaceStart + BaseHeaderSize) == (((eden).start))) {
      hackSlimBridgeToat(objectStartingAt(((eden).start)), pastSpaceStart);

      /* And carefully check the assumption */
      assert((objectAfterMaybeSlimBridgelimit(
                 objectInPastSpaceBefore(pastSpaceStart), nilObj)) ==
             (objectStartingAt(((eden).start))));
    } else {
      initSegmentBridgeWithBytesat((((eden).start)) - pastSpaceStart,
                                   pastSpaceStart);
    }
  }

  /* begin objectStartingAt: */
  numSlots = byteAt((void *)(start + (numSlotsFieldByteOffset())));
  objOop = (numSlots == (numSlotsMask()) ? start + BaseHeaderSize : start);
  while (oopisLessThan(objOop, freeStart)) {
    if (function(objOop)) {
      n += 1;
      printEntity(objOop);
    }
    prevPrevObj = prevObj;
    prevObj = objOop;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(objOop);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
      objOop = freeStart;
      goto l2;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOop =
        ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                 (numSlotsMask())
             ? ((oopisLessThan(objOop, oldSpaceStart)) &&
                        ((followingWord & 0xFFFFFFFFFFFFFFLL) == 1)
                    ? (followingWordAddress + BaseHeaderSize) + BaseHeaderSize
                    : followingWordAddress + BaseHeaderSize)
             : followingWordAddress);
    /* end objectAfterMaybeSlimBridge:limit: */
  l2:;
  }
  /* end allNewSpaceEntitiesDo: */
l3:
  if (n > 4) {
    printNum(n);
    print(" objects");
    cr();
  }
}