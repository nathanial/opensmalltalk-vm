/* Extracted from interp.c:40780 (function nilUnmarkedWeaklingSlots). */

/*	Nil the unmarked slots in the weaklings on the
        weakling stack, finalizing those that lost references.
        Finally, empty the weaklingStack. */
/*	for profiling */

/* SpurMemoryManager>>#nilUnmarkedWeaklingSlots */

static NeverInline void nilUnmarkedWeaklingSlots(void) {
  sqInt anyUnmarked;
  sqInt i;
  sqInt iSqInt;
  sqInt nextNextPage;
  sqInt nextPage;
  sqInt numOnThisPage;
  usqInt numSlots;
  sqInt numToEnumerate;
  sqInt numToEnumerateOnThisPage;
  sqInt objStackPage;
  sqInt referent;
  sqInt size;
  sqInt toDoLimit;
  sqInt topIndex;
  sqInt weakling;

  eassert(allOldMarkedWeakObjectsOnWeaklingStack());
  if (weaklingStack == nilObj) {
    return;
  }

  /* begin objStack:from:do: */
  eassert(isValidObjStack(weaklingStack));
  size = fetchPointerofObject(ObjStackTopx, weaklingStack);
  objStackPage = fetchPointerofObject(ObjStackNextx, weaklingStack);
  while (objStackPage != 0) {
    size += ObjStackLimit;
    assert((fetchPointerofObject(ObjStackTopx, objStackPage)) == ObjStackLimit);
    objStackPage = fetchPointerofObject(ObjStackNextx, objStackPage);
  }
  numToEnumerate = size;
  objStackPage = weaklingStack;
  while (numToEnumerate > 0) {
    numOnThisPage = fetchPointerofObject(ObjStackTopx, objStackPage);
    numToEnumerateOnThisPage =
        ((numToEnumerate < numOnThisPage) ? numToEnumerate : numOnThisPage);
    topIndex = (numOnThisPage + ObjStackFixedSlots) - 1;
    for (i = topIndex; i >= ((topIndex - numToEnumerateOnThisPage) + 1);
         i += -1) {
      assert(isWeak(fetchPointerofObject(i, objStackPage)));
      weakling = fetchPointerofObject(i, objStackPage);

      /* begin nilUnmarkedWeaklingSlotsIn: */
      anyUnmarked = 0;
      assert(allStrongSlotsOfWeaklingAreMarked(weakling));

      /* N.B. generateToByDoLimitExpression:negative:on: guards against
       * (unsigned)0 - 1 going +ve */
      toDoLimit = ((assert((classIndexOf(weakling)) >
                           (isForwardedObjectClassIndexPun())),
                    numSlotsOf(weakling))) -
                  1;
      for (iSqInt = (numStrongSlotsOfWeakling(weakling)); iSqInt <= toDoLimit;
           iSqInt += 1) {
        referent = fetchPointerofObject(iSqInt, weakling);
        if ((!(referent & (tagMask())))) {
          if (((longAt((void *)(referent))) & (classIndexMask())) ==
              (isForwardedObjectClassIndexPun())) {
            referent = fixFollowedFieldofObjectwithInitialValue(
                iSqInt, weakling, referent);
          }
          if (!((((referent & (tagMask())) != 0)) ||
                (((byteAt((void *)(referent + (markBitsByteOffset())))) &
                  (1U << (markedBitByteShift()))) != 0))) {
            /* begin storePointerUnchecked:ofObject:withValue: */
            assert((isNonImmediate(weakling)) && (!(isForwarded(weakling))));
            assert(validStorePointerUncheckedArgs(iSqInt, weakling, nilObj));
            longAtput((void *)((weakling + BaseHeaderSize) +
                               ((((usqInt)(iSqInt) << (shiftForWord()))))),
                      nilObj);
            anyUnmarked = 1;
          }
        }
      }
      if (anyUnmarked) {
        assert(marking);

        /* begin fireFinalization: */
        if (newFinalization) {
          queueMourner(weakling);
        }

        /* begin signalFinalization: */
        forceInterruptCheck();
        pendingFinalizationSignals += 1;
      }
    }
    numToEnumerate -= numToEnumerateOnThisPage;
    objStackPage = fetchPointerofObject(ObjStackNextx, objStackPage);
  }

  /* begin emptyObjStack: */
  if (weaklingStack == nilObj) {
    goto l1;
  }
  eassert(isValidObjStack(weaklingStack));

  /* begin storePointer:ofObjStack:withValue: */
  assert((formatOf(weaklingStack)) == (wordIndexableFormat()));
  longAtput((void *)((weaklingStack + BaseHeaderSize) +
                     ((((usqInt)(ObjStackTopx) << (shiftForWord()))))),
            0);
  nextPage = fetchPointerofObject(ObjStackNextx, weaklingStack);
  while (nextPage != 0) {
    nextNextPage = fetchPointerofObject(ObjStackNextx, nextPage);

    /* begin storePointer:ofObjStack:withValue: */
    assert((formatOf(nextPage)) == (wordIndexableFormat()));
    longAtput((void *)((nextPage + BaseHeaderSize) +
                       ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),
              fetchPointerofObject(ObjStackFreex, weaklingStack));

    /* begin storePointer:ofObjStack:withValue: */
    assert((formatOf(nextPage)) == (wordIndexableFormat()));
    longAtput((void *)((nextPage + BaseHeaderSize) +
                       ((((usqInt)(ObjStackNextx) << (shiftForWord()))))),
              0);

    /* begin storePointer:ofObjStack:withValue: */
    assert((formatOf(weaklingStack)) == (wordIndexableFormat()));
    longAtput((void *)((weaklingStack + BaseHeaderSize) +
                       ((((usqInt)(ObjStackFreex) << (shiftForWord()))))),
              nextPage);
    nextPage = nextNextPage;
  }

  /* begin storePointer:ofObjStack:withValue: */
  assert((formatOf(weaklingStack)) == (wordIndexableFormat()));
  longAtput((void *)((weaklingStack + BaseHeaderSize) +
                     ((((usqInt)(ObjStackNextx) << (shiftForWord()))))),
            0);
  eassert(isValidObjStack(weaklingStack));
  /* end emptyObjStack: */
l1:;
}