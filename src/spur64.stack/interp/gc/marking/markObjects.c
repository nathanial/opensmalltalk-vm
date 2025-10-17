/* Extracted from interp.c:40280 (function markObjects). */

/*	for profiling */
/*	Mark all accessible objects.
        objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged is true if all
        objects are unmarked and/or if unmarked classes shoud be removed from
   the class table. */
/*	If the incremental collector is running mark bits may be set; stop it
   and clear them if necessary.
 */

/* SpurMemoryManager>>#markObjects: */

static NeverInline void
markObjects(sqInt objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged) {
  sqInt classIndex;
  sqInt classOrNil;
  sqInt classTablePage;
  sqInt format;
  sqInt i;
  sqInt j;
  sqInt largestFree;
  sqInt objOop;
  sqInt oop;
  sqInt result;
  sqInt sizeOfAvailableNewSpace;
  sqInt sizeOfFutureSpace;
  sqInt sizeOfUnusedEden;
  StackPage *thePage;
  sqInt toDoLimit;

  runLeakCheckerFor(GCModeFull);

  /* begin shutDownGlobalIncrementalGC: */
  if (objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged) {
    assert(allObjectsUnmarked());
  }

  /* begin initializeUnscannedEphemerons */
  largestFree = findLargestFreeChunk();
  sizeOfUnusedEden = (((eden).limit)) - freeStart;
  sizeOfFutureSpace = (((futureSpace).limit)) - (((futureSpace).start));
  sizeOfAvailableNewSpace =
      ((sizeOfUnusedEden < sizeOfFutureSpace) ? sizeOfFutureSpace
                                              : sizeOfUnusedEden);
  if ((largestFree) && ((numSlotsOfAny(largestFree)) >
                        (sizeOfAvailableNewSpace / BytesPerWord))) {
    (unscannedEphemerons.start =
         (largestFree + BaseHeaderSize) +
         (5 /* (freeChunkLargerIndex + 1) */ * BytesPerWord));
    (unscannedEphemerons.limit = addressAfter(largestFree));
  } else {
    if (sizeOfUnusedEden > sizeOfFutureSpace) {
      (unscannedEphemerons.start = freeStart);
      (unscannedEphemerons.limit = ((eden).limit));
    } else {
      (unscannedEphemerons.start = ((futureSpace).start));
      (unscannedEphemerons.limit = ((futureSpace).limit));
    }
  }
  (unscannedEphemerons.top = ((unscannedEphemerons.start)) - BytesPerOop);

  /* begin initializeMarkStack */
  ensureRoomOnObjStackAt(MarkStackRootIndex);
  ensureRoomOnObjStackAt(WeaklingStackRootIndex);
  marking = 1;

  /* begin markAccessibleObjectsAndFireEphemerons */
  assert(marking);
  assert(validClassTableRootPages());
  assert(allBridgesMarked());

  /* for debugging markAndTrace: set (MarkStackRecord := OrderedCollection new)
     This must come first to enable stack page reclamation.  It clears
     the trace flags on stack pages and so must precede any marking.
     Otherwise it will clear the trace flags of reached pages. */

  /* begin initStackPageGC */
  if (stackPage) {
    /* begin externalWriteBackHeadFramePointers */
    assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
    assert(stackPage == (mostRecentlyUsedPage));
    assert(!((isFree(stackPage))));

    /* begin setHeadFP:andSP:inPage: */
    assert(stackPointer < framePointer);
    assert((stackPointer < ((stackPage->baseAddress))) &&
           (stackPointer > (((stackPage->realStackLimit)) -
                            (LargeContextSlots * BytesPerOop))));
    assert((framePointer < ((stackPage->baseAddress))) &&
           (framePointer > (((stackPage->realStackLimit)) -
                            ((LargeContextSlots * BytesPerOop) / 2))));
    (stackPage->headFP = framePointer);
    (stackPage->headSP = stackPointer);
    assert(pageListIsWellFormed());
  }
  for (i = 0; i < numStackPages; i += 1) {
    /* begin stackPageAt: */
    thePage = stackPageAtpages(i, pages);
    (thePage->trace = StackPageUnreached);
  }

  /* begin markAndTraceHiddenRoots */
  markAndTraceObjStackandContents(markStack, 0);
  markAndTraceObjStackandContents(weaklingStack, 0);
  markAndTraceObjStackandContents(mournQueue, 1);
  objOop = fetchPointerofObject(RememberedSetRootIndex, hiddenRootsObj);

  /* begin setIsMarkedOf:to: */
  assert(!(isFreeObject(objOop)));
  byteAtput((void *)(objOop + (markBitsByteOffset())),
            (byteAt((void *)(objOop + (markBitsByteOffset())))) |
                (1U << (markedBitByteShift())));
  /* begin freeListsObj */
  assert((firstIndexableField(oldSpaceObjectAfter(trueObj))) == freeLists);
  objOop = oldSpaceObjectAfter(trueObj);

  /* begin setIsMarkedOf:to: */
  assert(!(isFreeObject(objOop)));
  byteAtput((void *)(objOop + (markBitsByteOffset())),
            (byteAt((void *)(objOop + (markBitsByteOffset())))) |
                (1U << (markedBitByteShift())));

  /* begin isWeakNonImm: */
  format = (byteAt((void *)(classTableFirstPage + (formatFieldByteOffset())))) &
           (formatMask());
  if (format == (weakArrayFormat())) {
    markAndTrace(hiddenRootsObj);
    goto l1;
  }

  /* begin setIsMarkedOf:to: */
  assert(!(isFreeObject(hiddenRootsObj)));
  byteAtput((void *)(hiddenRootsObj + (markBitsByteOffset())),
            (byteAt((void *)(hiddenRootsObj + (markBitsByteOffset())))) |
                (1U << (markedBitByteShift())));
  markAndTrace(classTableFirstPage);
  for (i = 1; i < numClassTablePages; i += 1) {
    objOop = fetchPointerofObject(i, hiddenRootsObj);

    /* begin setIsMarkedOf:to: */
    assert(!(isFreeObject(objOop)));
    byteAtput((void *)(objOop + (markBitsByteOffset())),
              (byteAt((void *)(objOop + (markBitsByteOffset())))) |
                  (1U << (markedBitByteShift())));
  }
  /* end markAndTraceHiddenRoots */
l1:

  /* begin markAndTraceExtraRoots */
  assert(remapBufferCount == 0);

  /* 1 to: remapBufferCount do:
     [:i|
     oop := remapBuffer at: i.
     ((self isImmediate: oop) or: [self isFreeObject: oop]) ifFalse:
     [self markAndTrace: oop]]. */
  for (i = 1; i <= extraRootCount; i += 1) {
    oop = (extraRoots[i])[0];
    if (!((((oop & (tagMask())) != 0)) ||
          (((longAt((void *)(oop))) & (classIndexMask())) ==
           (isFreeObjectClassIndexPun())))) {
      markAndTrace(oop);
    }
  }
  assert(validClassTableRootPages());

  /* begin markAndTraceInterpreterOops: */
  markAndTraceStackPages(1);
  markAndTraceTraceLog();
  markAndTracePrimTraceLog();
  markAndTrace(specialObjectsOop);
  if (!(((newMethod & (tagMask())) != 0))) {
    markAndTrace(newMethod);
  }

  /* begin traceProfileState */
  /* begin followForwardingPointersInProfileState */
  /* begin profileStateDoUpdating: */
  if (profileProcess) {
    if ((result =
             ((!((longAt((void *)(profileProcess))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
                  ? followForwarded(profileProcess)
                  : 0))) {
      profileProcess = result;
    }
  }
  if (profileMethod) {
    if ((result =
             ((!((longAt((void *)(profileMethod))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
                  ? followForwarded(profileMethod)
                  : 0))) {
      profileMethod = result;
    }
  }
  if (profileSemaphore) {
    if ((result =
             ((!((longAt((void *)(profileSemaphore))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))
                  ? followForwarded(profileSemaphore)
                  : 0))) {
      profileSemaphore = result;
    }
  }

  /* begin profileStateDo: */
  if (profileProcess) {
    markAndTrace(profileProcess);
  }
  if (profileMethod) {
    markAndTrace(profileMethod);
  }
  if (profileSemaphore) {
    markAndTrace(profileSemaphore);
  }
#if LRPCheck
  sqLowLevelMFence();
  if ((longRunningPrimitiveCheckMethod != null) &&
      (longRunningPrimitiveCheckSequenceNumber != statCheckForEvents)) {
    if ((!((longAt((void *)(longRunningPrimitiveCheckMethod))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      longRunningPrimitiveCheckMethod =
          followForwarded(longRunningPrimitiveCheckMethod);
    }
    markAndTrace(longRunningPrimitiveCheckMethod);
  }
  if (longRunningPrimitiveCheckSemaphore) {
    if ((!((longAt((void *)(longRunningPrimitiveCheckSemaphore))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      longRunningPrimitiveCheckSemaphore =
          followForwarded(longRunningPrimitiveCheckSemaphore);
    }
    markAndTrace(longRunningPrimitiveCheckSemaphore);
  }
#endif // LRPCheck

  if (tempOop) {
    markAndTrace(tempOop);
  }
  if (tempOop2) {
    markAndTrace(tempOop2);
  }

  /* V3 memory manager support */
  for (i = 1; i <= remapBufferCount; i += 1) {
    oop = remapBuffer[i];
    if (!(((oop & (tagMask())) != 0))) {
      markAndTrace(oop);
    }
  }
  assert(validObjStacks());
  markWeaklingsAndMarkAndFireEphemerons();
  assert(validObjStacks());

  /* begin expungeDuplicateAndUnmarkedClasses: */
  for (i = 1; i < numClassTablePages; i += 1) {
    classTablePage = fetchPointerofObject(i, hiddenRootsObj);
    toDoLimit = (1U << (classTableMajorIndexShift())) - 1;
    for (j = 0; j <= toDoLimit; j += 1) {
      classOrNil = fetchPointerofObject(j, classTablePage);
      classIndex = ((((usqInt)(i) << (classTableMajorIndexShift())))) + j;
      assert((classOrNil == nilObj) || (addressCouldBeClassObj(classOrNil)));

      /* only remove a class if it is at a duplicate entry or it is unmarked and
       * we're expunging unmarked classes. */
      if (classOrNil == nilObj) {
        if (classIndex < classTableIndex) {
          classTableIndex = classIndex;
        }
      } else {
        if ((objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged &&
             (!(((byteAt((void *)(classOrNil + (markBitsByteOffset())))) &
                 (1U << (markedBitByteShift()))) != 0))) ||
            (((long32At((void *)(classOrNil + 4))) &
              (identityHashHalfWordMask())) != classIndex)) {
          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(classTablePage)) &&
                 (!(isForwarded(classTablePage))));
          assert(validStorePointerUncheckedArgs(j, classTablePage, nilObj));
          longAtput((void *)((classTablePage + BaseHeaderSize) +
                             ((((usqInt)(j) << (shiftForWord()))))),
                    nilObj);

          /* but if it is marked, it should still be in the table at its correct
           * index. */
          assert((objectsShouldBeUnmarkedAndUnmarkedClassesShouldBeExpunged &&
                  (!(isMarked(classOrNil)))) ||
                 ((classAtIndex(rawHashBitsOf(classOrNil))) == classOrNil));

          /* If the removed class is before the classTableIndex, set the
             classTableIndex to point to the empty slot so as to reuse it asap.
           */
          if (classIndex < classTableIndex) {
            classTableIndex = classIndex;
          }
        }
      }
    }
  }

  /* Avoid expunging the puns by not scanning the 0th page.
     classTableIndex must never index the first page, which is reserved for
     classes known to the VM. */
  assert(classTableIndex >= (1U << (classTableMajorIndexShift())));
  nilUnmarkedWeaklingSlots();
  marking = 0;
}