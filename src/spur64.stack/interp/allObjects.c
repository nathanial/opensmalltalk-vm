/* Extracted from interp.c:30412 (function allObjects). */

/*	Attempt to answer an array of all objects, excluding those that may
        be garbage collected as a side effect of allocating the result array.
        If no memory is available answer the number of objects as a
   SmallInteger. Since objects are at least 16 bytes big, and the largest
   SmallInteger covers 1/4 of the address space, the count can never overflow.
 */

/* SpurMemoryManager>>#allObjects */

static sqInt allObjects(void) {
  usqInt bytes;
  sqInt count;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt freeChunk;
  usqInt limit;
  usqInt numSlots;
  sqInt objOop;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt ptr;
  usqInt start;
  usqInt startUsqInt;

  /* N.B. Does /not/ update totalFreeOldSpace */
  freeChunk = allocateLargestFreeChunk();
  ptr = (start = freeChunk + BaseHeaderSize);
  limit = addressAfter(freeChunk);
  count = 0;

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
    if (((longAt((void *)(objOop))) & (classIndexMask())) >
        (lastClassIndexPun())) {
      count += 1;
      if (ptr < limit) {
        longAtput((void *)(ptr), objOop);
        ptr += BytesPerOop;
      }
    } else {
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
  startUsqInt = /* startAddressForBridgedHeapEnumeration */
      (pastSpaceStart > (((pastSpace).start))
           ? ((pastSpace).start)
           : (freeStart > (((eden).start)) ? ((eden).start) : oldSpaceStart));
  if (startUsqInt > freeStart) {
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
  numSlots = byteAt((void *)(startUsqInt + (numSlotsFieldByteOffset())));
  objOop = (numSlots == (numSlotsMask()) ? startUsqInt + BaseHeaderSize
                                         : startUsqInt);
  while (oopisLessThan(objOop, freeStart)) {
    if (((longAt((void *)(objOop))) & (classIndexMask())) >
        (lastClassIndexPun())) {
      count += 1;
      if (ptr < limit) {
        longAtput((void *)(ptr), objOop);
        ptr += BytesPerOop;
      }
    } else {
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

  /* continue enumerating even if no room so as to unmark all objects. */
  assert(isEmptyObjStack(markStack));
  assert(isEmptyObjStack(weaklingStack));
  assert(count >= (numSlotsMask()));
  if ((count > ((ptr - start) / BytesPerOop)) ||
      ((limit != ptr) && ((limit - ptr) <= 8 /* allocationUnit */))) {
    freeChunkWithBytesat(
        bytesInBody(freeChunk), /* startOfObject: */
        ((byteAt((void *)(freeChunk + (numSlotsFieldByteOffset())))) ==
                 (numSlotsMask())
             ? freeChunk - BaseHeaderSize
             : freeChunk));

    /* begin checkFreeSpace: */
    assert(bitsSetInFreeSpaceMaskForAllFreeLists());
    assert(totalFreeOldSpace == (totalFreeListBytes()));
    if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) ==
         (GCCheckFreeSpace | GCModeFull))) {
      runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
    }
    return (((usqInt)count << 3) | 1);
  }

  /* not enough room
     can't split a single word */
  bytes = (BaseHeaderSize + BaseHeaderSize) + (count * BytesPerOop);
  start = /* startOfObject: */
      ((byteAt((void *)(freeChunk + (numSlotsFieldByteOffset())))) ==
               (numSlotsMask())
           ? freeChunk - BaseHeaderSize
           : freeChunk);
  freeChunkWithBytesat((limit - start) - bytes, start + bytes);
  totalFreeOldSpace -= bytes;

  /* begin rawOverflowSlotsOf:put: */
  longAtput((void *)(freeChunk - BaseHeaderSize),
            ((((usqInt)((numSlotsMask())) << 56))) + count);

  /* begin set:classIndexTo:formatTo: */
  assert(((ClassArrayCompactIndex >= 0) &&
          (ClassArrayCompactIndex <= (classIndexMask()))));
  assert((((arrayFormat()) >= 0) && ((arrayFormat()) <= (formatMask()))));
  longAtput((void *)(freeChunk),
            ((longAt((void *)(freeChunk))) &
             (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) +
                             (classIndexMask())))) +
                (ClassArrayCompactIndex +
                 ((((usqInt)((arrayFormat())) << (formatShift()))))));

  /* begin possibleRootStoreInto: */
  if (!((byteAt((void *)(freeChunk + (formatFieldByteOffset())))) &
        (1U << (rememberedBitByteShift())))) {
    remember(freeChunk);
  }

  /* begin checkFreeSpace: */
  assert(bitsSetInFreeSpaceMaskForAllFreeLists());
  assert(totalFreeOldSpace == (totalFreeListBytes()));
  if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) ==
       (GCCheckFreeSpace | GCModeFull))) {
    runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
  }
  runLeakCheckerFor(GCModeFull);
  return freeChunk;
}