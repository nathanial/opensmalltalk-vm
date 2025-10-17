/* Extracted from interp.c:46933 (function compact). */

/*	Sweep all of old space, sliding unpinned marked objects down over free
   and unmarked objects. Let the segmentManager mark which segments contain
   pinned objects via notePinned:.
 */

/* SpurPlanningCompactor>>#compact */

static NeverInline void compact(void) {
  sqInt address;
  sqInt classIndex;
  sqInt finalPass;
  sqInt firstPass;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt highestSuitableFreeBlock;
  sqInt i;
  sqInt largestFreeChunk;
  sqInt largestFreeChunkSqInt;
  usqInt numSlots;
  sqInt objOopSqInt;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt sizeOfEden;
  sqInt spaceEstimate;
  sqInt stackOrNil;

  /* for profiling */
  /* begin initializeScanCheckingForFullyCompactedHeap */
  firstMobileObject = (lastMobileObject = (objectAfterLastMobileObject = null));
  reinitializeScanFrom(hiddenRootsObj);
  if (!firstFreeObject) {
    error("uncompactable heap; no unmarked objects found");
  }
  if (firstMobileObject >= endOfMemory) {
    /* begin unmarkObjectsInFullyCompactedHeap */
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
        goto l4;
      }
      followingWord = longAt((void *)(followingWordAddress));
      objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                             (numSlotsMask())
                         ? followingWordAddress + BaseHeaderSize
                         : followingWordAddress);
      /* end objectAfter:limit: */
    l4:;
    }
    /* end unmarkInitialImmobileObjects */
  l2:

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
        goto l3;
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
    l3:;
    }
    return;
  }

  /* begin initializeCompaction */
  /* begin checkFreeSpace: */
  assert(bitsSetInFreeSpaceMaskForAllFreeLists());
  assert(totalFreeOldSpace == (totalFreeListBytes()));
  if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) ==
       (GCCheckFreeSpace | GCModeFull))) {
    runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
  }

  /* begin selectSavedFirstFieldsSpace */
  spaceEstimate = (endOfMemory - nilObj) / 40;
  sizeOfEden = (((eden).limit)) - (((eden).start));
  if (spaceEstimate > sizeOfEden) {
    /* begin findHighestSuitableFreeBlock: */
    if ((largestFreeChunk = findLargestFreeChunk())) {
      if (((bytesInBody(largestFreeChunk)) >= spaceEstimate) &&
          ((((usqInt)largestFreeChunk)) >
           (((usqInt)(endOfMemory - totalFreeOldSpace))))) {
        highestSuitableFreeBlock = largestFreeChunk;
        goto l8;
      }
    }
    highestSuitableFreeBlock = null;
    /* end findHighestSuitableFreeBlock: */
  l8:
    if (highestSuitableFreeBlock) {
      if ((spaceEstimate > ((/* begin sizeOfFree: */
                             assert(isFreeObject(highestSuitableFreeBlock)),
                             bytesInBody(highestSuitableFreeBlock)))) &&
          (useSegmentForSavedFirstFieldsSpace(spaceEstimate))) {
        goto l5;
      }
      if (((/* begin sizeOfFree: */
            assert(isFreeObject(highestSuitableFreeBlock)),
            bytesInBody(highestSuitableFreeBlock))) > sizeOfEden) {
        /* begin useFreeChunkForSavedFirstFieldsSpace: */
        assert(validFreeTreeChunk(highestSuitableFreeBlock));
        (savedFirstFieldsSpace.start =
             highestSuitableFreeBlock +
             (4 /* freeChunkLargerIndex */ * BytesPerOop));
        (savedFirstFieldsSpace.limit = addressAfter(highestSuitableFreeBlock));
        savedFirstFieldsSpaceNotInOldSpace = 0;
        assert(!((savedFirstFieldsSpaceWasAllocated())));
        goto l5;
      }
    }
    if (useSegmentForSavedFirstFieldsSpace(spaceEstimate)) {
      goto l5;
    }
  }

  /* begin useEdenForSavedFirstFieldsSpace */
  (savedFirstFieldsSpace.start = ((eden).start));
  (savedFirstFieldsSpace.limit = ((eden).limit));
  savedFirstFieldsSpaceNotInOldSpace = 1;
  assert(!((savedFirstFieldsSpaceWasAllocated())));
  /* end selectSavedFirstFieldsSpace */
l5:

  /* begin unpinRememberedSet */
  /* begin fetchPointer:ofObject: */
  firstFieldOfRememberedSet =
      longAt((void *)(((longAt((void *)((hiddenRootsObj + BaseHeaderSize) +
                                        ((((usqInt)(RememberedSetRootIndex)
                                           << (shiftForWord()))))))) +
                       BaseHeaderSize) +
                      (0U << (shiftForWord()))));
  setIsPinnedOfto(longAt((void *)((hiddenRootsObj + BaseHeaderSize) +
                                  ((((usqInt)(RememberedSetRootIndex)
                                     << (shiftForWord())))))),
                  0);

  /* begin resetFreeListHeads */
  freeListsMask = 0;
  for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
    freeLists[i] = 0;
  }

  /* begin prepareObjStacksForPlanningCompactor */
  /* begin prepareObjStackForPlanningCompactor: */
  if (markStack == nilObj) {
    goto l7;
  }
  stackOrNil = markStack;
  do {
    assert((numSlotsOfAny(stackOrNil)) == ObjStackPageSlots);

    /* begin setHashBitsOf:to: */
    long32Atput((void *)(stackOrNil + 4),
                ((((long32At((void *)(stackOrNil + 4))) |
                   (identityHashHalfWordMask())) -
                  (identityHashHalfWordMask()))) +
                    ((longAt((void *)((stackOrNil + BaseHeaderSize) +
                                      ((((usqInt)(ObjStackTopx)
                                         << (shiftForWord()))))))) &
                     (identityHashHalfWordMask())));
  } while (((stackOrNil = longAt((void *)((stackOrNil + BaseHeaderSize) +
                                          ((((usqInt)(ObjStackNextx)
                                             << (shiftForWord())))))))) != 0);
  /* end prepareObjStackForPlanningCompactor: */
l7:
  if (weaklingStack == nilObj) {
    goto l9;
  }
  stackOrNil = weaklingStack;
  do {
    assert((numSlotsOfAny(stackOrNil)) == ObjStackPageSlots);

    /* begin setHashBitsOf:to: */
    long32Atput((void *)(stackOrNil + 4),
                ((((long32At((void *)(stackOrNil + 4))) |
                   (identityHashHalfWordMask())) -
                  (identityHashHalfWordMask()))) +
                    ((longAt((void *)((stackOrNil + BaseHeaderSize) +
                                      ((((usqInt)(ObjStackTopx)
                                         << (shiftForWord()))))))) &
                     (identityHashHalfWordMask())));
  } while (((stackOrNil = longAt((void *)((stackOrNil + BaseHeaderSize) +
                                          ((((usqInt)(ObjStackNextx)
                                             << (shiftForWord())))))))) != 0);
  /* end prepareObjStackForPlanningCompactor: */
l9:
  if (mournQueue == nilObj) {
    goto l6;
  }
  stackOrNil = mournQueue;
  do {
    assert((numSlotsOfAny(stackOrNil)) == ObjStackPageSlots);

    /* begin setHashBitsOf:to: */
    long32Atput((void *)(stackOrNil + 4),
                ((((long32At((void *)(stackOrNil + 4))) |
                   (identityHashHalfWordMask())) -
                  (identityHashHalfWordMask()))) +
                    ((longAt((void *)((stackOrNil + BaseHeaderSize) +
                                      ((((usqInt)(ObjStackTopx)
                                         << (shiftForWord()))))))) &
                     (identityHashHalfWordMask())));
  } while (((stackOrNil = longAt((void *)((stackOrNil + BaseHeaderSize) +
                                          ((((usqInt)(ObjStackNextx)
                                             << (shiftForWord())))))))) != 0);
  /* end prepareObjStackForPlanningCompactor: */
l6:
  totalFreeOldSpace = 0;
  gcPhaseInProgress = SlidingCompactionInProgress;
  (savedFirstFieldsSpace.top = ((savedFirstFieldsSpace.start)) - BytesPerOop);
  firstPass = 1;
  while (1) {
    finalPass = planCompactSavingForwarders();
    assert((validRelocationPlanInPass(finalPass)) == 0);
    updatePointers();
    copyAndUnmark(firstPass);

    /* Would like to check here, but can't if multi-pass. */

    /* Currently we do only a single pass if a normal GC, assuming that a pass
       will always compact plenty of space. But we should perhaps check this
       assumption by looking at the large free tree and seeing that the ratio of
       the largest free chunk to the total ammount of free space is high. */
    if (finalPass || (biasForGC))
      break;
    firstPass = 0;
    reinitializeScanFrom(firstFreeObject);
    if (!(firstMobileObject > firstFreeObject)) {
      firstFreeObject = firstMobileObject;
      unmarkObjectsFromFirstFreeObject();

      /* begin checkFreeSpace: */
      assert(bitsSetInFreeSpaceMaskForAllFreeLists());
      assert(totalFreeOldSpace == (totalFreeListBytes()));
      if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) ==
           (GCCheckFreeSpace | GCModeFull))) {
        runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
      }
      endCompaction();
      return;
    }

    /* We get here when firstFreeObject is the last object in memory.
       firstMobileObject may be random value from previous compaction
       in this case, and heap is fully compacted. Abort compaction.
       We set firstFreeObject to firstMobileObject for unmarking. */

    /* begin updateSavedFirstFieldsSpaceIfNecessary */
    if ((/* savedFirstFieldsSpaceInFreeChunk */
         (!savedFirstFieldsSpaceNotInOldSpace) &&
         (oopisGreaterThan((savedFirstFieldsSpace.start), nilObj))) &&
        (((savedFirstFieldsSpace.start)) < lastMobileObject)) {
      spaceEstimate =
          ((savedFirstFieldsSpace.limit)) - ((savedFirstFieldsSpace.start));

      /* begin findHighestSuitableFreeBlock: */
      if ((largestFreeChunkSqInt = findLargestFreeChunk())) {
        if (((bytesInBody(largestFreeChunkSqInt)) >= spaceEstimate) &&
            ((((usqInt)largestFreeChunkSqInt)) >
             (((usqInt)(endOfMemory - totalFreeOldSpace))))) {
          largestFreeChunk = largestFreeChunkSqInt;
          goto l1;
        }
      }
      largestFreeChunk = null;
      /* end findHighestSuitableFreeBlock: */
    l1:
      if (largestFreeChunk) {
        /* begin useFreeChunkForSavedFirstFieldsSpace: */
        assert(validFreeTreeChunk(largestFreeChunk));
        (savedFirstFieldsSpace.start =
             largestFreeChunk + (4 /* freeChunkLargerIndex */ * BytesPerOop));
        (savedFirstFieldsSpace.limit = addressAfter(largestFreeChunk));
        savedFirstFieldsSpaceNotInOldSpace = 0;
        assert(!((savedFirstFieldsSpaceWasAllocated())));
      } else {
        /* begin useEdenForSavedFirstFieldsSpace */
        (savedFirstFieldsSpace.start = ((eden).start));
        (savedFirstFieldsSpace.limit = ((eden).limit));
        savedFirstFieldsSpaceNotInOldSpace = 1;
        assert(!((savedFirstFieldsSpaceWasAllocated())));
      }
    }
    (savedFirstFieldsSpace.top = ((savedFirstFieldsSpace.start)) - BytesPerOop);
  }

  /* begin checkFreeSpace: */
  assert(bitsSetInFreeSpaceMaskForAllFreeLists());
  assert(totalFreeOldSpace == (totalFreeListBytes()));
  if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) ==
       (GCCheckFreeSpace | GCModeFull))) {
    runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
  }
  endCompaction();
}