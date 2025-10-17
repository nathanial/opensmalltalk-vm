/* Extracted from interp.c:29935 (function allInstancesOf). */

/*	Attempt to answer an array of all objects, excluding those that may
        be garbage collected as a side effect of allocating the result array.
        If no memory is available answer the number of instances as a
        SmallInteger. Since objects are at least 16 bytes big, and the largest
        SmallInteger covers
        1/4 of the address space, the count can never overflow. */

/* SpurMemoryManager>>#allInstancesOf: */

static sqInt allInstancesOf(sqInt aClass) {
  sqInt actualIndex;
  usqInt bytes;
  sqInt classIndex;
  sqInt classOrNil;
  sqInt count;
  sqInt countSqInt;
  sqInt expectedIndex;
  sqInt expectedIndexSqInt;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt freeChunk;
  sqInt i;
  sqInt index;
  sqInt iSqInt;
  sqInt j;
  usqInt limit;
  usqInt newObj;
  usqInt numBytes;
  usqInt numSlots;
  sqInt objOop;
  sqInt page;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt ptr;
  sqInt ptrSqInt;
  sqInt smallObj;
  usqInt start;
  usqInt startUsqInt;
  sqInt toDoLimit;
  sqInt valuePointer;

  classIndex = (long32At((void *)(aClass + 4))) & (identityHashHalfWordMask());
  if (!classIndex) {
    /* begin allocateSlots:format:classIndex: */
    newObj = freeStart;
    numBytes = BaseHeaderSize + 8 /* allocationUnit */;
    if ((freeStart + numBytes) > scavengeThreshold) {
      if (0 <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
        if (!needGCFlag) {
          /* begin scheduleScavenge */
          needGCFlag = 1;
          forceInterruptCheck();
        }
      }
      freeChunk = allocateSlotsInOldSpacebytesformatclassIndex(
          0, numBytes, arrayFormat(), ClassArrayCompactIndex);
      goto l1;
    }
    longAtput((void *)(newObj),
              ((((((usqLong)0)) << (numSlotsFullShift()))) +
               ((((usqInt)((arrayFormat())) << (formatShift()))))) +
                  ClassArrayCompactIndex);

    /* for header parsing we put a saturated slot count in the prepended
     * overflow size word */
    assert((numBytes % (allocationUnit())) == 0);
    assert((newObj % (allocationUnit())) == 0);
    freeStart += numBytes;
    freeChunk = newObj;
    /* end allocateSlots:format:classIndex: */
  l1:
    return freeChunk;
  }

  /* N.B. Does /not/ update totalFreeOldSpace */
  freeChunk = allocateLargestFreeChunk();
  start = freeChunk + BaseHeaderSize;
  limit = addressAfter(freeChunk);

  /* begin isClassAtUniqueIndex: */
  expectedIndex =
      (long32At((void *)(aClass + 4))) & (identityHashHalfWordMask());
  for (iSqInt = 0; iSqInt < numClassTablePages; iSqInt += 1) {
    page = fetchPointerofObject(iSqInt, hiddenRootsObj);
    toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
    for (j = 0; j <= toDoLimit; j += 1) {
      classOrNil = fetchPointerofObject(j, page);
      if (classOrNil != nilObj) {
        index = ((((usqInt)(iSqInt) << (classTableMajorIndexShift())))) + j;
        if ((classOrNil == aClass) &&
            ((index != expectedIndex) && (index > (lastClassIndexPun())))) {
          goto l4;
        }
      }
    }
  }

  /* begin uniqueIndex:allInstancesInto:limit:resultsInto: */
  countSqInt = 0;
  ptrSqInt = start;

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
      if (((longAt((void *)(objOop))) & (classIndexMask())) == classIndex) {
        countSqInt += 1;
        if (ptrSqInt < limit) {
          longAtput((void *)(ptrSqInt), objOop);
          ptrSqInt += BytesPerOop;
        }
      }
    } else {
    }
    prevPrevObj = prevObj;
    prevObj = objOop;

    /* begin objectAfter:limit: */
    followingWordAddress = addressAfter(objOop);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objOop = endOfMemory;
      goto l10;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                      (numSlotsMask())
                  ? followingWordAddress + BaseHeaderSize
                  : followingWordAddress);
    /* end objectAfter:limit: */
  l10:;
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
    goto l9;
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
      if (((longAt((void *)(objOop))) & (classIndexMask())) == classIndex) {
        countSqInt += 1;
        if (ptrSqInt < limit) {
          longAtput((void *)(ptrSqInt), objOop);
          ptrSqInt += BytesPerOop;
        }
      }
    } else {
    }
    prevPrevObj = prevObj;
    prevObj = objOop;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(objOop);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
      objOop = freeStart;
      goto l8;
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
  l8:;
  }
  /* end allNewSpaceEntitiesDo: */
l9:

  /* continue enumerating even if no room so as to unmark all objects. */
  count = countSqInt;
  ptr = ptrSqInt;
  goto l3;
l4:

  /* begin ambiguousClass:allInstancesInto:limit:resultsInto: */
  countSqInt = 0;
  ptrSqInt = start;
  expectedIndex =
      (long32At((void *)(aClass + 4))) & (identityHashHalfWordMask());

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
      actualIndex = (longAt((void *)(objOop))) & (classIndexMask());
      if ((classOrNilAtIndex(actualIndex)) == aClass) {
        if (actualIndex != expectedIndex) {
          /* begin setClassIndexOf:to: */
          assert(
              ((expectedIndex >= 0) && (expectedIndex <= (classIndexMask()))));
          longAtput((void *)(objOop), ((longAt((void *)(objOop))) &
                                       (~(usqIntptr_t)(classIndexMask()))) +
                                          expectedIndex);
        }
        countSqInt += 1;
        if (ptrSqInt < limit) {
          longAtput((void *)(ptrSqInt), objOop);
          ptrSqInt += BytesPerOop;
        }
      }
    } else {
    }
    prevPrevObj = prevObj;
    prevObj = objOop;

    /* begin objectAfter:limit: */
    followingWordAddress = addressAfter(objOop);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objOop = endOfMemory;
      goto l7;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                      (numSlotsMask())
                  ? followingWordAddress + BaseHeaderSize
                  : followingWordAddress);
    /* end objectAfter:limit: */
  l7:;
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
    goto l6;
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
      actualIndex = (longAt((void *)(objOop))) & (classIndexMask());
      if ((classOrNilAtIndex(actualIndex)) == aClass) {
        if (actualIndex != expectedIndex) {
          /* begin setClassIndexOf:to: */
          assert(
              ((expectedIndex >= 0) && (expectedIndex <= (classIndexMask()))));
          longAtput((void *)(objOop), ((longAt((void *)(objOop))) &
                                       (~(usqIntptr_t)(classIndexMask()))) +
                                          expectedIndex);
        }
        countSqInt += 1;
        if (ptrSqInt < limit) {
          longAtput((void *)(ptrSqInt), objOop);
          ptrSqInt += BytesPerOop;
        }
      }
    } else {
    }
    prevPrevObj = prevObj;
    prevObj = objOop;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(objOop);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, freeStart)) {
      objOop = freeStart;
      goto l5;
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
  l5:;
  }
  /* end allNewSpaceEntitiesDo: */
l6:

  /* continue enumerating even if no room so as to unmark all objects and/or
   * normalize class indices. */

  /* begin purgeDuplicateClassTableEntriesFor: */
  expectedIndexSqInt =
      (long32At((void *)(aClass + 4))) & (identityHashHalfWordMask());
  for (iSqInt = 0; iSqInt < numClassTablePages; iSqInt += 1) {
    page = fetchPointerofObject(iSqInt, hiddenRootsObj);
    toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
    for (j = 0; j <= toDoLimit; j += 1) {
      classOrNil = fetchPointerofObject(j, page);
      if (classOrNil != nilObj) {
        index = ((((usqInt)(iSqInt) << (classTableMajorIndexShift())))) + j;
        if ((classOrNil == aClass) && ((index != expectedIndexSqInt) &&
                                       (index > (lastClassIndexPun())))) {
          classAtIndexput(index, nilObj);
          if (index < classTableIndex) {
            classTableIndex = index;
          }
        }
      }
    }
  }

  /* classTableIndex must never index the first page, which is reserved for
   * classes known to the VM. */
  assert(classTableIndex >= (1U << (classTableMajorIndexShift())));
  count = countSqInt;
  ptr = ptrSqInt;
l3:
  assert(isEmptyObjStack(markStack));
  assert(isEmptyObjStack(weaklingStack));
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
  if (count < (numSlotsMask())) {
    /* begin allocateSlots:format:classIndex: */
    if (count >= (numSlotsMask())) {
      if ((((usqInt)(count)) >> 56) > 0) {
        smallObj = null;
        goto l2;
      }
      newObj = freeStart + BaseHeaderSize;
      numBytes = (BaseHeaderSize + BaseHeaderSize) + (count * BytesPerOop);
    } else {
      newObj = freeStart;
      numBytes = BaseHeaderSize + ((count < 1 ? 8 /* allocationUnit */
                                              : count * BytesPerOop));
    }
    if ((freeStart + numBytes) > scavengeThreshold) {
      if (count <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
        if (!needGCFlag) {
          /* begin scheduleScavenge */
          needGCFlag = 1;
          forceInterruptCheck();
        }
      }
      smallObj = allocateSlotsInOldSpacebytesformatclassIndex(
          count, numBytes, arrayFormat(), ClassArrayCompactIndex);
      goto l2;
    }
    if (count >= (numSlotsMask())) {
      longAtput((void *)(freeStart),
                ((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) +
                    count);
      longAtput((void *)(newObj),
                ((((((usqLong)(numSlotsMask()))) << (numSlotsFullShift()))) +
                 ((((usqInt)((arrayFormat())) << (formatShift()))))) +
                    ClassArrayCompactIndex);
    } else {
      longAtput((void *)(newObj),
                ((((((usqLong)count)) << (numSlotsFullShift()))) +
                 ((((usqInt)((arrayFormat())) << (formatShift()))))) +
                    ClassArrayCompactIndex);
    }

    /* for header parsing we put a saturated slot count in the prepended
     * overflow size word */
    assert((numBytes % (allocationUnit())) == 0);
    assert((newObj % (allocationUnit())) == 0);
    freeStart += numBytes;
    smallObj = newObj;
    /* end allocateSlots:format:classIndex: */
  l2:
    for (i = 0; i < count; i += 1) {
      valuePointer = fetchPointerofObject(i, freeChunk);

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(smallObj)) && (!(isForwarded(smallObj))));
      assert(validStorePointerUncheckedArgs(i, smallObj, valuePointer));
      longAtput((void *)((smallObj + BaseHeaderSize) +
                         ((((usqInt)(i) << (shiftForWord()))))),
                valuePointer);
    }
    freeChunkWithBytesat(
        bytesInBody(freeChunk), /* startOfObject: */
        ((byteAt((void *)(freeChunk + (numSlotsFieldByteOffset())))) ==
                 (numSlotsMask())
             ? freeChunk - BaseHeaderSize
             : freeChunk));
    beRootIfOld(smallObj);

    /* begin checkFreeSpace: */
    assert(bitsSetInFreeSpaceMaskForAllFreeLists());
    assert(totalFreeOldSpace == (totalFreeListBytes()));
    if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) ==
         (GCCheckFreeSpace | GCModeFull))) {
      runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
    }
    return smallObj;
  }
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