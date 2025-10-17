/* Extracted from interp.c:48178 (function updatePointers). */

/*	Sweep the heap, updating all objects to their eventual locations.
        Remember to update the savedFirstFields of pointer objects, as these
   have been forwarded. */

/* SpurPlanningCompactor>>#updatePointers */

static NeverInline void updatePointers(void) {
  sqInt address;
  sqInt availableSpace;
  usqInt bytes;
  sqInt classIndex;
  sqInt contextSize;
  sqInt fmt;
  sqInt followingWord;
  usqInt followingWordAddress;
  sqInt fwd;
  sqInt header;
  sqInt heapEntity;
  sqInt i;
  usqInt numLiterals;
  usqInt numPointerSlots;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt objOop;
  sqInt objOopSqInt;
  sqInt onePass;
  sqInt oop;
  usqInt previousPin;
  sqInt prevObj;
  sqInt prevPrevObj;
  sqInt sp;
  usqInt startOfPreviousPin;
  usqInt toFinger;
  usqInt top;

  if (!(lastMobileObject)) {
    return;
  }
  assert((startOfObject(firstFreeObject)) == mobileStart);
  mapInterpreterOops();
  mapExtraRoots();

  /* begin updatePointersInManagerHeapEntities */
  /* begin relocateObjStacksForPlanningCompactor */
  markStack = relocateObjStackForPlanningCompactorandContents(markStack, 0);
  weaklingStack =
      relocateObjStackForPlanningCompactorandContents(weaklingStack, 0);
  mournQueue = relocateObjStackForPlanningCompactorandContents(mournQueue, 1);
  if ((rememberedSetSize > 0) &&
      (/* isMobile: */
       (oopisGreaterThanOrEqualToandLessThanOrEqualTo(
           firstFieldOfRememberedSet, mobileStart, lastMobileObject)) &&
       (!(((byteAt((void *)(firstFieldOfRememberedSet +
                            (formatFieldByteOffset())))) &
           (1U << (pinnedBitByteShift()))) != 0)))) {
    firstFieldOfRememberedSet =
        fetchPointerofObject(0U, firstFieldOfRememberedSet);
  }
  heapEntity = fetchPointerofObject(RememberedSetRootIndex, hiddenRootsObj);

  /* begin relocateObjectsInHeapEntity:from:to: */
  for (i = 1; i < rememberedSetSize; i += 1) {
    oop = fetchPointerofObject(i, heapEntity);
    if (((!(oop & (tagMask())))) &&
        (/* isMobile: */
         (oopisGreaterThanOrEqualToandLessThanOrEqualTo(oop, mobileStart,
                                                        lastMobileObject)) &&
         (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) &
             (1U << (pinnedBitByteShift()))) != 0)))) {
      assert(isMarked(oop));
      fwd = fetchPointerofObject(0U, oop);
      assert(isPostMobile(fwd));

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(heapEntity)) && (!(isForwarded(heapEntity))));
      assert(validStorePointerUncheckedArgs(i, heapEntity, fwd));
      longAtput((void *)((heapEntity + BaseHeaderSize) +
                         ((((usqInt)(i) << (shiftForWord()))))),
                fwd);
    }
  }
  if (/* isMobile: */
      (oopisGreaterThanOrEqualToandLessThanOrEqualTo(heapEntity, mobileStart,
                                                     lastMobileObject)) &&
      (!(((byteAt((void *)(heapEntity + (formatFieldByteOffset())))) &
          (1U << (pinnedBitByteShift()))) != 0))) {
    /* fetchPointer:ofObject: */
    fetchPointerofObject(0U, heapEntity);
  } else {
  }

  /* begin updatePointersInSurvivingObjects */
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

    /* begin updatePointersIn: */
    /* begin numPointerSlotsOf: */
    fmt = (byteAt((void *)(objOopSqInt + (formatFieldByteOffset())))) &
          (formatMask());
    if (fmt <= 5 /* lastPointerFormat */) {
      if ((fmt == (indexablePointersFormat())) &&
          (((longAt((void *)(objOopSqInt))) & (classIndexMask())) ==
           ClassMethodContextCompactIndex)) {
        /* begin fetchStackPointerOf: */
        sp = fetchPointerofObject(StackPointerIndex, objOopSqInt);
        if (!((((sp) & 7) == 1))) {
          contextSize = 0;
          goto l3;
        }
        assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(objOopSqInt)));
        contextSize = (sp >> 3);
        /* end fetchStackPointerOf: */
      l3:
        numPointerSlots = CtxtTempFrameStart + contextSize;
        goto l2;
      }

      /* contexts end at the stack pointer */

      /* begin numSlotsOf: */
      assert((classIndexOf(objOopSqInt)) > (isForwardedObjectClassIndexPun()));
      numPointerSlots =
          (((numSlots = byteAt(
                 (void *)(objOopSqInt + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)((
                     (sqInt)((usqInt)((longAt(
                                 (void *)(objOopSqInt - BaseHeaderSize))))
                             << 8)))))) >>
                     8
               : numSlots);
      goto l2;
    }
    if (fmt == (forwardedFormat())) {
      numPointerSlots = 1;
      goto l2;
    }
    if (fmt < (firstCompiledMethodFormat())) {
      numPointerSlots = 0;
      goto l2;
    }

    /* begin methodHeaderOf: */
    assert(isCompiledMethod(objOopSqInt));
    header = fetchPointerofObject(HeaderIndex, objOopSqInt);

    /* begin literalCountOfMethodHeader: */
    assert((((header) & 7) == 1));
    numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
    numPointerSlots = numLiterals + LiteralStart;
    /* end numPointerSlotsOf: */
  l2:
    for (i = 0; i < numPointerSlots; i += 1) {
      oop = fetchPointerofObject(i, objOopSqInt);
      if (((!(oop & (tagMask())))) &&
          (/* isMobile: */
           (oopisGreaterThanOrEqualToandLessThanOrEqualTo(oop, mobileStart,
                                                          lastMobileObject)) &&
           (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) &
               (1U << (pinnedBitByteShift()))) != 0)))) {
        assert((isMarked(oop)) || (objOopSqInt == (hiddenRootsObject())));
        fwd = fetchPointerofObject(0U, oop);
        assert(isPostMobile(fwd));

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(objOopSqInt)) && (!(isForwarded(objOopSqInt))));
        assert(validStorePointerUncheckedArgs(i, objOopSqInt, fwd));
        longAtput((void *)((objOopSqInt + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  fwd);
      }
    }
    prevPrevObj = prevObj;
    prevObj = objOopSqInt;

    /* begin objectAfterMaybeSlimBridge:limit: */
    followingWordAddress = addressAfter(objOopSqInt);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, pastSpaceStart)) {
      objOopSqInt = pastSpaceStart;
      goto l1;
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
  l1:;
  }

  /* begin updatePointersInInitialImmobileObjects */
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
      if (oopisGreaterThanOrEqualTo(objOopSqInt, firstFreeObject)) {
        goto l7;
      }

      /* would like to assert this, but it isn't true if more than one pass:
       * self assert: (manager isMarked: o). */

      /* begin updatePointersIn: */
      /* begin numPointerSlotsOf: */
      fmt = (byteAt((void *)(objOopSqInt + (formatFieldByteOffset())))) &
            (formatMask());
      if (fmt <= 5 /* lastPointerFormat */) {
        if ((fmt == (indexablePointersFormat())) &&
            (((longAt((void *)(objOopSqInt))) & (classIndexMask())) ==
             ClassMethodContextCompactIndex)) {
          /* begin fetchStackPointerOf: */
          sp = fetchPointerofObject(StackPointerIndex, objOopSqInt);
          if (!((((sp) & 7) == 1))) {
            contextSize = 0;
            goto l4;
          }
          assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(objOopSqInt)));
          contextSize = (sp >> 3);
          /* end fetchStackPointerOf: */
        l4:
          numPointerSlots = CtxtTempFrameStart + contextSize;
          goto l6;
        }

        /* contexts end at the stack pointer */

        /* begin numSlotsOf: */
        assert((classIndexOf(objOopSqInt)) >
               (isForwardedObjectClassIndexPun()));
        numPointerSlots =
            (((numSlots = byteAt(
                   (void *)(objOopSqInt + (numSlotsFieldByteOffset()))))) ==
                     (numSlotsMask())
                 ? ((((usqInt)((
                       (sqInt)((usqInt)((longAt(
                                   (void *)(objOopSqInt - BaseHeaderSize))))
                               << 8)))))) >>
                       8
                 : numSlots);
        goto l6;
      }
      if (fmt == (forwardedFormat())) {
        numPointerSlots = 1;
        goto l6;
      }
      if (fmt < (firstCompiledMethodFormat())) {
        numPointerSlots = 0;
        goto l6;
      }

      /* begin methodHeaderOf: */
      assert(isCompiledMethod(objOopSqInt));
      header = fetchPointerofObject(HeaderIndex, objOopSqInt);

      /* begin literalCountOfMethodHeader: */
      assert((((header) & 7) == 1));
      numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
      numPointerSlots = numLiterals + LiteralStart;
      /* end numPointerSlotsOf: */
    l6:
      for (i = 0; i < numPointerSlots; i += 1) {
        oop = fetchPointerofObject(i, objOopSqInt);
        if (((!(oop & (tagMask())))) &&
            (/* isMobile: */
             (oopisGreaterThanOrEqualToandLessThanOrEqualTo(
                 oop, mobileStart, lastMobileObject)) &&
             (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) &
                 (1U << (pinnedBitByteShift()))) != 0)))) {
          assert((isMarked(oop)) || (objOopSqInt == (hiddenRootsObject())));
          fwd = fetchPointerofObject(0U, oop);
          assert(isPostMobile(fwd));

          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(objOopSqInt)) &&
                 (!(isForwarded(objOopSqInt))));
          assert(validStorePointerUncheckedArgs(i, objOopSqInt, fwd));
          longAtput((void *)((objOopSqInt + BaseHeaderSize) +
                             ((((usqInt)(i) << (shiftForWord()))))),
                    fwd);
        }
      }
    }
    prevPrevObj = prevObj;
    prevObj = objOopSqInt;

    /* begin objectAfter:limit: */
    followingWordAddress = addressAfter(objOopSqInt);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objOopSqInt = endOfMemory;
      goto l5;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                           (numSlotsMask())
                       ? followingWordAddress + BaseHeaderSize
                       : followingWordAddress);
    /* end objectAfter:limit: */
  l5:;
  }
  /* end updatePointersInInitialImmobileObjects */
l7:

  /* begin updatePointersInMobileObjects */
  availableSpace = 0;
  previousPin = 0;
  assert(!((isMarked(firstFreeObject))));
  toFinger = /* startOfObject: */
      ((byteAt((void *)(firstFreeObject + (numSlotsFieldByteOffset())))) ==
               (numSlotsMask())
           ? firstFreeObject - BaseHeaderSize
           : firstFreeObject);
  top = (savedFirstFieldsSpace.start);
  startOfPreviousPin = 0;

  /* begin allOldSpaceEntitiesFrom:do: */
  assert(isOldObject(firstFreeObject));
  prevPrevObj = (prevObj = null);
  objOop = firstFreeObject;
  while (1) {
    assert((objOop % (allocationUnit())) == 0);
    if (!(oopisLessThan(objOop, endOfMemory)))
      break;
    assert((long64At((void *)(objOop))) != 0);
    assert((previousPin == null
                ? toFinger <= (startOfObject(objOop))
                : (isMarked(previousPin)) && (toFinger <= startOfPreviousPin)));
    if ((byteAt((void *)(objOop + (markBitsByteOffset())))) &
        (1U << (markedBitByteShift()))) {
      if ((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
          (1U << (pinnedBitByteShift()))) {
        if (!previousPin) {
          previousPin = objOop;
          startOfPreviousPin = /* startOfObject: */
              ((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) ==
                       (numSlotsMask())
                   ? objOop - BaseHeaderSize
                   : objOop);
        }

        /* begin updatePointersIn: */
        /* begin numPointerSlotsOf: */
        fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) &
              (formatMask());
        if (fmt <= 5 /* lastPointerFormat */) {
          if ((fmt == (indexablePointersFormat())) &&
              (((longAt((void *)(objOop))) & (classIndexMask())) ==
               ClassMethodContextCompactIndex)) {
            /* begin fetchStackPointerOf: */
            sp = longAt((
                void *)((objOop + BaseHeaderSize) +
                        ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
            if (!((((sp) & 7) == 1))) {
              contextSize = 0;
              goto l8;
            }
            assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(objOop)));
            contextSize = (sp >> 3);
            /* end fetchStackPointerOf: */
          l8:
            numPointerSlots = CtxtTempFrameStart + contextSize;
            goto l10;
          }

          /* contexts end at the stack pointer */

          /* begin numSlotsOf: */
          assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun()));
          numPointerSlots =
              (((numSlotsUsqInt =
                     byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) ==
                       (numSlotsMask())
                   ? ((((usqInt)((
                         (sqInt)((usqInt)((
                                     longAt((void *)(objOop - BaseHeaderSize))))
                                 << 8)))))) >>
                         8
                   : numSlotsUsqInt);
          goto l10;
        }
        if (fmt == (forwardedFormat())) {
          numPointerSlots = 1;
          goto l10;
        }
        if (fmt < (firstCompiledMethodFormat())) {
          numPointerSlots = 0;
          goto l10;
        }

        /* begin methodHeaderOf: */
        assert(isCompiledMethod(objOop));
        header = fetchPointerofObject(HeaderIndex, objOop);

        /* begin literalCountOfMethodHeader: */
        assert((((header) & 7) == 1));
        numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
        numPointerSlots = numLiterals + LiteralStart;
        /* end numPointerSlotsOf: */
      l10:
        for (i = 0; i < numPointerSlots; i += 1) {
          oop = fetchPointerofObject(i, objOop);
          if (((!(oop & (tagMask())))) &&
              (/* isMobile: */
               (oopisGreaterThanOrEqualToandLessThanOrEqualTo(
                   oop, mobileStart, lastMobileObject)) &&
               (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) &
                   (1U << (pinnedBitByteShift()))) != 0)))) {
            assert((isMarked(oop)) || (objOop == (hiddenRootsObject())));
            fwd = fetchPointerofObject(0U, oop);
            assert(isPostMobile(fwd));

            /* begin storePointerUnchecked:ofObject:withValue: */
            assert((isNonImmediate(objOop)) && (!(isForwarded(objOop))));
            assert(validStorePointerUncheckedArgs(i, objOop, fwd));
            longAtput((void *)((objOop + BaseHeaderSize) +
                               ((((usqInt)(i) << (shiftForWord()))))),
                      fwd);
          }
        }
      } else {
        bytes = bytesInBody(objOop);
        while ((toFinger <= startOfPreviousPin) &&
               ((bytes != ((availableSpace = startOfPreviousPin - toFinger))) &&
                ((bytes + (16)) > availableSpace))) {
          do {
            toFinger = addressAfter(previousPin);

            /* begin objectStartingAt: */
            numSlots = byteAt((void *)(toFinger + (numSlotsFieldByteOffset())));
            previousPin =
                (numSlots == (numSlotsMask()) ? toFinger + BaseHeaderSize
                                              : toFinger);
          } while (
              (((byteAt((void *)(previousPin + (markBitsByteOffset())))) &
                (1U << (markedBitByteShift()))) != 0) &&
              ((((byteAt((void *)(previousPin + (formatFieldByteOffset())))) &
                 (1U << (pinnedBitByteShift()))) != 0) &&
               (previousPin < objOop)));

          /* Now previousPin is either equal to o or mobile.
             Move it to the next pinned object below o */
          while (
              !((previousPin >= objOop) ||
                ((((byteAt((void *)(previousPin + (markBitsByteOffset())))) &
                   (1U << (markedBitByteShift()))) != 0) &&
                 (((byteAt((void *)(previousPin + (formatFieldByteOffset())))) &
                   (1U << (pinnedBitByteShift()))) != 0)))) {
            previousPin = oldSpaceObjectAfter(previousPin);
          }
          if (previousPin >= objOop) {
            previousPin = null;
            startOfPreviousPin = 0;
          } else {
            startOfPreviousPin = /* startOfObject: */
                ((byteAt(
                     (void *)(previousPin + (numSlotsFieldByteOffset())))) ==
                         (numSlotsMask())
                     ? previousPin - BaseHeaderSize
                     : previousPin);
          }
        }

        /* The object does not fit in the space between toFinger and
           previousPin. Move toFinger up to point at the first unmarked or
           mobile object after previousPin, or, if previousPin is contiguous
           with o, to the start of this object.  Update previousPin to be the
           next pinned object above toFInger and below this object, or nil if no
           such pinned object exists. Any unfillable gaps between adjacent
           pinned objects will be freed. */
        updatePointersInsavedFirstFieldPointer(objOop, top);
        toFinger += bytes;
        if (((top += BytesPerOop)) >= ((savedFirstFieldsSpace.limit))) {
          assert(((savedFirstFieldsSpace.top)) == (top - BytesPerOop));
          onePass = 0;
          goto l11;
        }
      }
    }
    prevPrevObj = prevObj;
    prevObj = objOop;

    /* begin objectAfter:limit: */
    followingWordAddress = addressAfter(objOop);
    if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
      objOop = endOfMemory;
      goto l9;
    }
    followingWord = longAt((void *)(followingWordAddress));
    objOop = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                      (numSlotsMask())
                  ? followingWordAddress + BaseHeaderSize
                  : followingWordAddress);
    /* end objectAfter:limit: */
  l9:;
  }
  assert(((savedFirstFieldsSpace.top)) == (top - BytesPerOop));
  onePass = 1;
  /* end updatePointersInMobileObjects */
l11:
  if (!onePass) {
    /* begin updatePointersInObjectsOverflowingSavedFirstFieldsSpace */
    /* begin allOldSpaceObjectsFrom:do: */
    /* begin allOldSpaceEntitiesFrom:do: */
    assert(isOldObject(objectAfterLastMobileObject));
    prevPrevObj = (prevObj = null);
    objOopSqInt = objectAfterLastMobileObject;
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
        if ((byteAt((void *)(objOopSqInt + (markBitsByteOffset())))) &
            (1U << (markedBitByteShift()))) {
          /* begin updatePointersIn: */
          /* begin numPointerSlotsOf: */
          fmt = (byteAt((void *)(objOopSqInt + (formatFieldByteOffset())))) &
                (formatMask());
          if (fmt <= 5 /* lastPointerFormat */) {
            if ((fmt == (indexablePointersFormat())) &&
                (((longAt((void *)(objOopSqInt))) & (classIndexMask())) ==
                 ClassMethodContextCompactIndex)) {
              /* begin fetchStackPointerOf: */
              sp = fetchPointerofObject(StackPointerIndex, objOopSqInt);
              if (!((((sp) & 7) == 1))) {
                contextSize = 0;
                goto l12;
              }
              assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(objOopSqInt)));
              contextSize = (sp >> 3);
              /* end fetchStackPointerOf: */
            l12:
              numPointerSlots = CtxtTempFrameStart + contextSize;
              goto l14;
            }

            /* contexts end at the stack pointer */

            /* begin numSlotsOf: */
            assert((classIndexOf(objOopSqInt)) >
                   (isForwardedObjectClassIndexPun()));
            numPointerSlots =
                (((numSlots = byteAt(
                       (void *)(objOopSqInt + (numSlotsFieldByteOffset()))))) ==
                         (numSlotsMask())
                     ? ((((usqInt)((
                           (sqInt)((usqInt)((longAt(
                                       (void *)(objOopSqInt - BaseHeaderSize))))
                                   << 8)))))) >>
                           8
                     : numSlots);
            goto l14;
          }
          if (fmt == (forwardedFormat())) {
            numPointerSlots = 1;
            goto l14;
          }
          if (fmt < (firstCompiledMethodFormat())) {
            numPointerSlots = 0;
            goto l14;
          }

          /* begin methodHeaderOf: */
          assert(isCompiledMethod(objOopSqInt));
          header = fetchPointerofObject(HeaderIndex, objOopSqInt);

          /* begin literalCountOfMethodHeader: */
          assert((((header) & 7) == 1));
          numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
          numPointerSlots = numLiterals + LiteralStart;
          /* end numPointerSlotsOf: */
        l14:
          for (i = 0; i < numPointerSlots; i += 1) {
            oop = fetchPointerofObject(i, objOopSqInt);
            if (((!(oop & (tagMask())))) &&
                (/* isMobile: */
                 (oopisGreaterThanOrEqualToandLessThanOrEqualTo(
                     oop, mobileStart, lastMobileObject)) &&
                 (!(((byteAt((void *)(oop + (formatFieldByteOffset())))) &
                     (1U << (pinnedBitByteShift()))) != 0)))) {
              assert((isMarked(oop)) || (objOopSqInt == (hiddenRootsObject())));
              fwd = fetchPointerofObject(0U, oop);
              assert(isPostMobile(fwd));

              /* begin storePointerUnchecked:ofObject:withValue: */
              assert((isNonImmediate(objOopSqInt)) &&
                     (!(isForwarded(objOopSqInt))));
              assert(validStorePointerUncheckedArgs(i, objOopSqInt, fwd));
              longAtput((void *)((objOopSqInt + BaseHeaderSize) +
                                 ((((usqInt)(i) << (shiftForWord()))))),
                        fwd);
            }
          }
        }
      }
      prevPrevObj = prevObj;
      prevObj = objOopSqInt;

      /* begin objectAfter:limit: */
      followingWordAddress = addressAfter(objOopSqInt);
      if (oopisGreaterThanOrEqualTo(followingWordAddress, endOfMemory)) {
        objOopSqInt = endOfMemory;
        goto l13;
      }
      followingWord = longAt((void *)(followingWordAddress));
      objOopSqInt = ((((usqInt)(followingWord)) >> (numSlotsFullShift())) ==
                             (numSlotsMask())
                         ? followingWordAddress + BaseHeaderSize
                         : followingWordAddress);
      /* end objectAfter:limit: */
    l13:;
    }
  }
}