/* Extracted from interp.c:40556 (function
 * markWeaklingsAndMarkAndFireEphemerons). */

/*	After the initial scan-mark is complete ephemerons can be processed.
        Weaklings have accumulated on the weaklingStack, but more may be
        uncovered during ephemeron processing. So trace the strong slots
        of the weaklings, and as ephemerons are processed ensure any newly
        reached weaklings are also traced. */

/* SpurMemoryManager>>#markWeaklingsAndMarkAndFireEphemerons */

static void markWeaklingsAndMarkAndFireEphemerons(void) {
  sqInt ephemeron;
  sqInt field;
  sqInt i;
  sqInt iSqInt;
  sqInt key;
  sqInt numOnThisPage;
  sqInt numToEnumerate;
  sqInt numToEnumerateOnThisPage;
  sqInt numTracedWeaklings;
  sqInt objStackPage;
  usqInt p;
  sqInt size;
  sqInt toDoLimit;
  sqInt topIndex;
  sqInt weakling;

  numTracedWeaklings = 0;
  while (1) {
    markAndTraceUntracedReachableStackPages();

    /* Make sure all reached weaklings have their strong slots traced before
     * firing ephemerons... */
    do {
      /* begin markAndTraceWeaklingsFrom: */
      /* begin objStack:from:do: */
      eassert(isValidObjStack(weaklingStack));
      size = fetchPointerofObject(ObjStackTopx, weaklingStack);
      objStackPage =
          fetchPointerofObject(ObjStackNextx, weaklingStack);
      while (objStackPage != 0) {
        size += ObjStackLimit;
        assert((fetchPointerofObject(ObjStackTopx, objStackPage)) ==
               ObjStackLimit);
        objStackPage =
            fetchPointerofObject(ObjStackNextx, objStackPage);
      }
      numToEnumerate = size - numTracedWeaklings;
      objStackPage = weaklingStack;
      while (numToEnumerate > 0) {
        numOnThisPage =
            fetchPointerofObject(ObjStackTopx, objStackPage);
        numToEnumerateOnThisPage =
            ((numToEnumerate < numOnThisPage) ? numToEnumerate : numOnThisPage);
        topIndex = (numOnThisPage + ObjStackFixedSlots) - 1;
        for (iSqInt = topIndex;
             iSqInt >= ((topIndex - numToEnumerateOnThisPage) + 1);
             iSqInt += -1) {
          assert(isWeak(fetchPointerofObject(iSqInt, objStackPage)));
          weakling =
              fetchPointerofObject(iSqInt, objStackPage);
          assert(!((isForwarded(weakling))));
          markAndTraceClassOf(weakling);

          /* N.B. generateToByDoLimitExpression:negative:on: guards against
           * (unsigned)0 - 1 going +ve */
          toDoLimit = (numStrongSlotsOfWeakling(weakling)) - 1;
          for (i = 0; i <= toDoLimit; i += 1) {
            /* begin followOopField:ofObject: */
            field = fetchPointerofObject(i, weakling);
            if (isOopForwarded(field)) {
              field =
                  fixFollowedFieldofObjectwithInitialValue(i, weakling, field);
            }
            if (!((((field & (tagMask())) != 0)) ||
                  (((byteAt((void *)(field + (markBitsByteOffset())))) &
                    (1U << (markedBitByteShift()))) != 0))) {
              markAndTrace(field);
            }
          }
        }
        numToEnumerate -= numToEnumerateOnThisPage;
        objStackPage =
            fetchPointerofObject(ObjStackNextx, objStackPage);
      }
      numTracedWeaklings = size;
    } while ((sizeOfObjStack(weaklingStack)) > numTracedWeaklings);
    if (((unscannedEphemerons.top)) < ((unscannedEphemerons.start))) {
      markAndTraceUntracedReachableStackPages();
      freeUntracedStackPages();
      return;
    }
    if (!(markInactiveEphemerons())) {
      /* begin fireAllUnscannedEphemerons */
      assert(!(noUnscannedEphemerons()));
      assert(allUnscannedEphemeronsAreActive());
      for (p = ((unscannedEphemerons.start)); p <= ((unscannedEphemerons.top));
           p += BytesPerOop) {
        fireEphemeron(longAt((void *)(p)));
      }
    }

    /* begin markAllUnscannedEphemerons */
    assert(allUnscannedEphemeronsAreActive());
    while (((unscannedEphemerons.top)) >= ((unscannedEphemerons.start))) {
      ephemeron = longAt((void *)((unscannedEphemerons.start)));
      longAtput((void *)((unscannedEphemerons.start)),
                longAt((void *)((unscannedEphemerons.top))));
      (unscannedEphemerons.top = ((unscannedEphemerons.top)) - BytesPerOop);

      /* begin followedKeyOfMaybeFiredEphemeron: */
      assert((isNonImmediate(ephemeron)) && (isMaybeFiredEphemeron(ephemeron)));

      /* begin followOopField:ofObject: */
      key = fetchPointerofObject(0U, ephemeron);
      if (isOopForwarded(key)) {
        key = fixFollowedFieldofObjectwithInitialValue(0, ephemeron, key);
      }

      /* begin setIsMarkedOf:to: */
      assert(!(isFreeObject(ephemeron)));
      byteAtput((void *)(ephemeron + (markBitsByteOffset())),
                (byteAt((void *)(ephemeron + (markBitsByteOffset())))) &
                    (0xFF - (1U << (markedBitByteShift()))));
      markAndTrace(key);
      markAndTrace(ephemeron);
    }
  }
}