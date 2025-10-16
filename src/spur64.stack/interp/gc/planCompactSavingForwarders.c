/* Extracted from interp.c:47738 (function planCompactSavingForwarders). */

/*	Sweep the heap from firstFreeObject forwarding marked objects to where
        they can be moved to, saving their forwarding pointer in
        savedFirstFieldsSpace. Continue until either the end of the heap is
        reached or savedFirstFieldsSpace is full.
        Answer if the end of the heap was reached (savedFirstFieldsSpace has not
        overflowed).
        The enumerations in planCompactSavingForwarders,
        updatePointersInMobileObjects and copyAndUnmarkMobileObjects
        match. We could implement them as a single enumeration method taking
        several block arguments, but arguably that
        would make understanding an already tricky algorithm more difficult.
        Instead we tolerate the duplication and encourage
        the reader to diff the three methods to see where they diverge (e.g. via
        Cmd-shift-C).  */

/* SpurPlanningCompactor>>#planCompactSavingForwarders */

static NeverInline sqInt planCompactSavingForwarders(void) {
  sqInt availableSpace;
  usqInt bytes;
  sqInt eventualLocation;
  sqInt followingWord;
  usqInt followingWordAddress;
  usqInt numSlots;
  sqInt objOop;
  usqInt previousPin;
  sqInt prevObj;
  sqInt prevPrevObj;
  usqInt startOfPreviousPin;
  usqInt toFinger;
  usqInt top;

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
    assert(savedFirstFieldsSpaceNotInOldSpace || (toFinger < top));
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

        /* begin forwardMobileObject:to:savedFirstFieldPtr: */
        lastMobileObject = objOop;
        eventualLocation =
            ((byteAt((void *)(objOop + (numSlotsFieldByteOffset())))) ==
                     (numSlotsMask())
                 ? toFinger + BaseHeaderSize
                 : toFinger);
        longAtput((void *)(top), fetchPointerofObject(0U, objOop));

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(objOop)) && (!(isForwarded(objOop))));
        assert(validStorePointerUncheckedArgs(0, objOop, eventualLocation));
        longAtput(
            (void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))),
            eventualLocation);
        toFinger += bytes;
        if (((top += BytesPerOop)) >= ((savedFirstFieldsSpace.limit))) {
          (savedFirstFieldsSpace.top = top - BytesPerOop);
          objectAfterLastMobileObject = oldSpaceObjectAfter(lastMobileObject);
          return 0;
        }
      }
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

  /* If the heap is already fully compacted there will be no lastMobileObject...
   */
  if (lastMobileObject) {
    (savedFirstFieldsSpace.top = top - BytesPerOop);
    objectAfterLastMobileObject = oldSpaceObjectAfter(lastMobileObject);
  }
  return 1;
}