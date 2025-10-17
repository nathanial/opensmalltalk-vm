/* Extracted from interp.c:36486 (function initializeObjectMemory). */

/*	Initialize object memory variables at startup time. Assume endOfMemory
   at al are initialised by the image-reading code via
        setHeapBase:memoryLimit:endOfMemory:. endOfMemory is assumed to point to
        the end of the last object in the image.
        Assume: image reader also initializes the following variables:
        specialObjectsOop
        lastHash */
/*	Catch mis-initializations leading to bad translations to C */

/* SpurMemoryManager>>#initializeObjectMemory: */

static void initializeObjectMemory(sqInt bytesToShift) {
  sqInt anOop;
  sqInt cameFrom;
  usqInt chunkBytes;
  sqInt classIndex;
  sqInt classTableRoot;
  sqInt field;
  sqInt fieldAddr;
  sqInt fieldOop;
  sqInt firstObj;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt freeChunk;
  sqInt freeListObj;
  usqInt freeOldStart;
  sqInt fudge;
  sqInt i;
  sqInt index;
  sqInt j;
  sqInt largeChild;
  usqInt limit;
  sqInt min;
  sqInt nilObjPreSwizzle;
  usqInt numSlots;
  sqInt obj;
  usqInt p;
  sqInt page;
  sqInt smallChild;
  usqInt startOfFreeOldSpace;
  usqInt toDoLimit;
  sqInt treeNode;
  sqInt valuePointer;

  assert(BaseHeaderSize == BaseHeaderSize);
  assert((((sqInt)((maxSlotsForAlloc()) * BytesPerWord))) > 0);
  initSegmentBridgeWithBytesat(2 * BaseHeaderSize,
                               endOfMemory - (2 * BaseHeaderSize));
  adjustSegmentSwizzlesBy(bytesToShift);

  /* image may be at a different address; adjust oops for new location */

  /* begin adjustAllOopsBy: */
  assert(newSpaceIsEmpty());
  numSlots = byteAt((void *)(oldSpaceStart + (numSlotsFieldByteOffset())));
  firstObj = (numSlots == (numSlotsMask()) ? oldSpaceStart + BaseHeaderSize
                                           : oldSpaceStart);
  classTableRoot = oldSpaceObjectAfter(
      oldSpaceObjectAfter(oldSpaceObjectAfter(oldSpaceObjectAfter(firstObj))));
  nilObjPreSwizzle = oldSpaceStart - bytesToShift;

  /* begin numSlotsOf: */
  assert((classIndexOf(classTableRoot)) > (isForwardedObjectClassIndexPun()));
  numClassTablePages =
      (((numSlots =
             byteAt((void *)(classTableRoot + (numSlotsFieldByteOffset()))))) ==
               (numSlotsMask())
           ? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(classTableRoot -
                                                            BaseHeaderSize))))
                                  << 8)))))) >>
                 8
           : numSlots);
  assert(numClassTablePages == ((classTableRootSlots()) + (hiddenRootSlots())));
  for (i = 2; i < numClassTablePages; i += 1) {
    if ((longAt((void *)((classTableRoot + BaseHeaderSize) +
                         ((((usqInt)(i) << (shiftForWord()))))))) ==
        nilObjPreSwizzle) {
      numClassTablePages = i;
      goto l4;
    }
  }
  /* end countNumClassPagesPreSwizzle: */
l4:
  if ((bytesToShift != 0) || (numSegments > 1)) {
    /* begin objectStartingAt: */
    numSlots = byteAt((void *)(oldSpaceStart + (numSlotsFieldByteOffset())));
    obj = (numSlots == (numSlotsMask()) ? oldSpaceStart + BaseHeaderSize
                                        : oldSpaceStart);
    while (oopisLessThan(obj, freeOldSpaceStart)) {
      classIndex = (longAt((void *)(obj))) & (classIndexMask());
      if (classIndex >= (isForwardedObjectClassIndexPun())) {
        /* begin swizzleFieldsOfObject: */
        fieldAddr = obj + (lastPointerOfWhileSwizzling(obj));
        while (oopisGreaterThanOrEqualTo(fieldAddr, obj + BaseHeaderSize)) {
          fieldOop = longAt((void *)(fieldAddr));
          if ((!(fieldOop & (tagMask())))) {
            longAtput((void *)(fieldAddr), swizzleObj(fieldOop));
          }
          fieldAddr -= BytesPerOop;
        }
      } else {
        if (classIndex == (isFreeObjectClassIndexPun())) {
          /* begin swizzleFieldsOfFreeChunk: */
          field = longAt(
              (void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))));
          if (field) {
            valuePointer = swizzleObj(field);

            /* begin storeUncheckedNoAssert:ofAny:withValue: */
            longAtput(
                (void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))),
                valuePointer);
          }
          chunkBytes = bytesInBody(obj);

          /* The prevPointer is not guaranteed to be valid in older images.
             updateListStartingAt: via updateFreeLists does restore the prev
             pointer in all small free lists, so simply avoid swizzling it now.
           */
          if (chunkBytes >= 0x200 /* (numFreeLists * #allocationUnit) */) {
            for (index = 2 /* freeChunkParentIndex */;
                 index <= 4 /* freeChunkLargerIndex */; index += 1) {
              field =
                  longAt((void *)((obj + BaseHeaderSize) +
                                  ((((usqInt)(index) << (shiftForWord()))))));
              if (field) {
                valuePointer = swizzleObj(field);

                /* begin storeUncheckedNoAssert:ofAny:withValue: */
                longAtput((void *)((obj + BaseHeaderSize) +
                                   ((((usqInt)(index) << (shiftForWord()))))),
                          valuePointer);
              }
            }
          }
        }
      }
      followingWordAddress = addressAfter(obj);
      if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
        obj = endOfMemory;
        goto l3;
      }
      followingWord = longAt((void *)(followingWordAddress));
      obj = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                     (numSlotsMask())
                 ? followingWordAddress + BaseHeaderSize
                 : followingWordAddress);
      /* end objectAfter:limit: */
    l3:;
    }
  }
  specialObjectsOop = swizzleObj(specialObjectsOop);

  /* heavily used special objects */
  nilObj = longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                           ((((usqInt)(NilObject) << (shiftForWord()))))));
  falseObj = longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                             ((((usqInt)(FalseObject) << (shiftForWord()))))));
  trueObj = longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                            ((((usqInt)(TrueObject) << (shiftForWord()))))));

  /* In Cog we insist that nil, true & false are next to each other (Cogit
     generates tighter conditional branch code as a result).  In addition, Spur
     places the free lists and class table root page immediately following them.
   */
  assert(nilObj == oldSpaceStart);
  assert(falseObj == (oldSpaceObjectAfter(nilObj)));
  assert(trueObj == (oldSpaceObjectAfter(falseObj)));
  freeListObj = oldSpaceObjectAfter(trueObj);
  anOop = oldSpaceObjectAfter(freeListObj);

  /* begin setHiddenRootsObj: */
  hiddenRootsObj = anOop;
  assert(validClassTableRootPages());
  classTableFirstPage = longAt(
      (void *)((hiddenRootsObj + BaseHeaderSize) + (0U << (shiftForWord()))));
  assert(((numSlotsOf(classTableFirstPage)) - 1) ==
         (classTableMinorIndexMask()));

  /* Hack fix.  A bug in markAndTraceClassOf: caused the class of the first
     class table page to be changed from its pun.  This can be restored
     manually, but we do it here too. */
  if (((longAt((void *)(classTableFirstPage))) & (classIndexMask())) !=
      (arrayClassIndexPun())) {
    /* begin setClassIndexOf:to: */
    assert((((arrayClassIndexPun()) >= 0) &&
            ((arrayClassIndexPun()) <= (classIndexMask()))));
    longAtput((void *)(classTableFirstPage),
              ((longAt((void *)(classTableFirstPage))) &
               (~(usqIntptr_t)(classIndexMask()))) +
                  (arrayClassIndexPun()));
  }
  numClassTablePages =
      1U << (22 /* classIndexFieldWidth */ - (classTableMajorIndexShift()));

  /* Set classTableIndex to the start of the last used page (excepting first
     page). Set numClassTablePages to the number of used pages. If loading an
     image, set the classTableIndex to the first unused slot in the class table
     after the first page. Set numClassTablePages to the number of used pages.
     Set classTableIndex to point at the first unused entry. First set it to the
     max as a sentinel. */
  classTableIndex =
      ((sqInt)((usqInt)(numClassTablePages) << (classTableMajorIndexShift())));
  for (i = 1; i < numClassTablePages; i += 1) {
    if (((page = longAt((void *)((hiddenRootsObj + BaseHeaderSize) +
                                 ((((usqInt)(i) << (shiftForWord())))))))) ==
        nilObj) {
      if ((((usqInt)(classTableIndex)) >> (classTableMajorIndexShift())) > i) {
        classTableIndex = ((sqInt)((usqInt)(((((i - 1) < 1) ? 1 : (i - 1))))
                                   << (classTableMajorIndexShift())));
      }
      numClassTablePages = i;
      assert((classOrNilAtIndex(classTableIndex)) == nilObj);
      goto l1;
    } else {
      if ((((usqInt)(classTableIndex)) >> (classTableMajorIndexShift())) > i) {
        j = 0;
        while (j < (1U << (classTableMajorIndexShift()))) {
          if ((longAt((void *)((page + BaseHeaderSize) +
                               ((((usqInt)(j) << (shiftForWord()))))))) ==
              nilObj) {
            classTableIndex =
                ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
            j = 1U << (classTableMajorIndexShift());
          }
          j += 1;
        }
      }
    }
  }

  /* no unused slots; set it to the start of the second page. */
  if ((((usqInt)(classTableIndex)) >> (classTableMajorIndexShift())) >=
      numClassTablePages) {
    classTableIndex = 1U << (classTableMajorIndexShift());
  }
  assert((classOrNilAtIndex(classTableIndex)) == nilObj);
  /* end setHiddenRootsObj: */
l1:
  markStack = swizzleObjStackAt(MarkStackRootIndex);
  weaklingStack = swizzleObjStackAt(WeaklingStackRootIndex);
  mournQueue = swizzleObjStackAt(MournQueueRootIndex);
  assert(validObjStacks());
  assert(isEmptyObjStack(markStack));
  assert(isEmptyObjStack(weaklingStack));

  /* begin initializeFreeSpacePostLoad: */
  assert((numSlotsOf(freeListObj)) == (numFreeLists()));
  assert((formatOf(freeListObj)) == (wordIndexableFormat()));
  freeLists = firstIndexableField(freeListObj);
  freeListsMask = 0;
  for (i = 0; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
    if (freeLists[i]) {
      freeListsMask = freeListsMask | (1ULL << i);
      freeLists[i] = (swizzleObj(freeLists[i]));
    }
  }
  collapseSegmentsPostSwizzle();

  /* begin updateFreeLists */
  min = 3;
  for (i = min; i <= 0x3F /* (numFreeLists - 1) */; i += 1) {
    updateListStartingAt(freeLists[i]);
  }

  /* Large chunks */

  /* begin freeTreeNodesDo: */
  treeNode = freeLists[0];
  if (!treeNode) {
    goto l2;
  }
  cameFrom = -1;
  do {
    assert((bytesInBody(treeNode)) >= ((numFreeLists()) * (allocationUnit())));
    smallChild = longAt(
        (void *)((treeNode + BaseHeaderSize) + (3U << (shiftForWord()))));
    largeChild = longAt(
        (void *)((treeNode + BaseHeaderSize) + (4U << (shiftForWord()))));
    assert((smallChild == 0) ||
           (treeNode ==
            (fetchPointerofFreeChunk(freeChunkParentIndex(), smallChild))));
    assert((largeChild == 0) ||
           (treeNode ==
            (fetchPointerofFreeChunk(freeChunkParentIndex(), largeChild))));

    /* apply if the node has no children, or it has no large children and we're
       returning from the small child, or we're returning from the large child.
     */
    if (((smallChild == 0) && (largeChild == 0)) ||
        ((largeChild ? cameFrom == largeChild : cameFrom == smallChild))) {
      updateListStartingAt(treeNode);

      /* and since we've applied we must move on up */
      cameFrom = treeNode;
      treeNode = longAt(
          (void *)((treeNode + BaseHeaderSize) + (2U << (shiftForWord()))));
    } else {
      if ((smallChild != 0) && (cameFrom != smallChild)) {
        treeNode = smallChild;
      } else {
        assert(largeChild != 0);
        treeNode = largeChild;
      }
      cameFrom = -1;
    }
  } while (treeNode != 0);
  /* end freeTreeNodesDo: */
l2:

  /* begin computeFreeSpacePostSwizzle */
  totalFreeOldSpace = totalFreeListBytes();
  assert(bitsSetInFreeSpaceMaskForAllFreeLists());
  assert(totalFreeOldSpace == (totalFreeListBytes()));
  startOfFreeOldSpace = freeOldSpaceStart;

  /* begin initializeOldSpaceFirstFree: */
  limit = endOfMemory - (2 * BaseHeaderSize);
  if (limit > startOfFreeOldSpace) {
    totalFreeOldSpace += limit - startOfFreeOldSpace;
    freeOldStart = startOfFreeOldSpace;
    while ((limit - freeOldStart) >= (0x100000000LL)) {
      freeChunk = freeChunkWithBytesat(0x100000000LL, freeOldStart);
      freeOldStart += 0x100000000LL;
      assert(freeOldStart == (addressAfter(freeChunk)));
    }
    if (freeOldStart < limit) {
      freeChunk = freeChunkWithBytesat(limit - freeOldStart, freeOldStart);
      assert((addressAfter(freeChunk)) == limit);
    }
  }
  endOfMemory -= 2 * BaseHeaderSize;
  freeOldSpaceStart = endOfMemory;

  /* begin checkFreeSpace: */
  assert(bitsSetInFreeSpaceMaskForAllFreeLists());
  assert(totalFreeOldSpace == (totalFreeListBytes()));
  if (((checkForLeaks & (GCCheckFreeSpace | GCCheckFreeSpace)) ==
       (GCCheckFreeSpace | GCCheckFreeSpace))) {
    runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
  }
  initializeNewSpaceVariables();

  /* begin initializeRememberedSet */
  obj = longAt(
      (void *)((hiddenRootsObj + BaseHeaderSize) +
               ((((usqInt)(RememberedSetRootIndex) << (shiftForWord()))))));
  if (obj == nilObj) {
    /* begin allocatePinnedSlots: */
    obj = allocateSlotsForPinningInOldSpacebytesformatclassIndex(
        0x400, (1024U << (shiftForWord())) + (BaseHeaderSize + BaseHeaderSize),
        sixtyFourBitIndexableFormat(), sixtyFourBitLongsClassIndexPun());
    if (obj) {
      assert(isPinned(obj));

      /* begin fillObj:numSlots:with: */
      assert(oopisLessThan(((obj + BaseHeaderSize) + (0x400 * BytesPerOop)) - 1,
                           addressAfter(obj)));
      toDoLimit =
          ((usqInt)(((obj + BaseHeaderSize) + (0x400 * BytesPerOop)) - 1));
      for (p = (((usqInt)(obj + BaseHeaderSize))); p <= toDoLimit;
           p += 8 /* allocationUnit */) {
        longAtput((void *)(p), 0);
      }
    }

    /* begin rememberedSetObj: */
    assert(isOldObject(obj));
    assert((isNonImmediate(hiddenRootsObj)) &&
           (!(isForwarded(hiddenRootsObj))));
    assert(validStorePointerUncheckedArgs(RememberedSetRootIndex,
                                          hiddenRootsObj, obj));
    longAtput(
        (void *)((hiddenRootsObj + BaseHeaderSize) +
                 ((((usqInt)(RememberedSetRootIndex) << (shiftForWord()))))),
        obj);
  } else {
    /* begin setFormatOf:to: */
    assert((((sixtyFourBitIndexableFormat()) >= 0) &&
            ((sixtyFourBitIndexableFormat()) <= (formatMask()))));
    byteAtput((void *)(obj + (formatFieldByteOffset())),
              ((byteAt((void *)(obj + (formatFieldByteOffset())))) &
               (0xFF - (formatMask()))) +
                  (sixtyFourBitIndexableFormat()));
  }

  /* The Spur32to64BitBootstrap failed to set the type of rememberedSetObj to
     64-bit indexability. This is unimportant except for simulation;
     rememberedSet is declared as sqInt *, but to have firstIndexableField:
     below answer a suitable type the format must be wordIndexableFormat. */
  assert((formatOf(obj)) == (wordIndexableFormat()));
  assert(isPinned(obj));
  rememberedSet = firstIndexableField(obj);
  rememberedSetSize = 0;

  /* begin numSlotsOf: */
  assert((classIndexOf(obj)) > (isForwardedObjectClassIndexPun()));
  rememberedSetLimit =
      (((numSlots = byteAt((void *)(obj + (numSlotsFieldByteOffset()))))) ==
               (numSlotsMask())
           ? ((((usqInt)((
                 (sqInt)((usqInt)((longAt((void *)(obj - BaseHeaderSize))))
                         << 8)))))) >>
                 8
           : numSlots);

  /* begin setRememberedSetRedZone */
  fudge = ((((eden.limit)) - ((eden.start))) / BytesPerWord) / 0x400;
  rememberedSetRedZone = ((((rememberedSetLimit * 3) / 4) < fudge)
                              ? fudge
                              : ((rememberedSetLimit * 3) / 4));
  checkSegments();

  /* begin biasForGC */
  biasForGC = 1;

  /* These defaults should depend on machine size; e.g. too small on a powerful
   * laptop, too big on a Pi. */

  /* headroom when growing */
  growHeadroom = 0x1000000;

  /* free space before shrinking */
  shrinkThreshold = 0x2000000;
  heapSizeAtPreviousGC =
      ((assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
        totalHeapSizeIncludingBridges - (numSegments * (2 * BaseHeaderSize)))) -
      totalFreeOldSpace;

  /* begin resetAllocationAccountingAfterGC */
  oldSpaceUsePriorToScavenge =
      ((assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges),
        totalHeapSizeIncludingBridges - (numSegments * (2 * BaseHeaderSize)))) -
      totalFreeOldSpace;

  /* By default GC after scavenge if heap has grown by a third since the last GC
   */
  heapGrowthToSizeGCRatio = 0.333333;
}