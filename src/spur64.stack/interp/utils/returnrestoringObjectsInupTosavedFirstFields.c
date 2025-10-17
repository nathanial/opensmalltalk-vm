/* Extracted from interp.c:45178 (function
 * returnrestoringObjectsInupTosavedFirstFields). */

/*	This is part of storeImageSegmentInto:outPointers:roots:. */

/* SpurMemoryManager>>#return:restoringObjectsIn:upTo:savedFirstFields: */

static sqInt returnrestoringObjectsInupTosavedFirstFields(
    sqInt errCode, sqInt reachableObjectsArray, sqInt limitOrTag,
    sqInt savedFirstFields) {
  sqInt i;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt oop;
  sqInt valuePointer;

  /* begin restoreObjectsIn:upTo:savedFirstFields: */
  numSlots =
      (limitOrTag == -1
           ? (
              assert((classIndexOf(reachableObjectsArray)) >
                     (isForwardedObjectClassIndexPun())),
              numSlotsOf(reachableObjectsArray))
           : limitOrTag);
  for (i = 0; i < numSlots; i += 1) {
    oop = fetchPointerofObject(i, reachableObjectsArray);
    valuePointer = fetchPointerofObject(i, savedFirstFields);

    /* begin storePointerUnchecked:ofObject:withValue: */
    assert((isNonImmediate(oop)) && (!(isForwarded(oop))));
    assert(validStorePointerUncheckedArgs(0, oop, valuePointer));
    longAtput((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))),
              valuePointer);

    /* begin setIsMarkedOf:to: */
    assert(!(isFreeObject(oop)));
    byteAtput((void *)(oop + (markBitsByteOffset())),
              (byteAt((void *)(oop + (markBitsByteOffset())))) &
                  (0xFF - (1U << (markedBitByteShift()))));
  }
  if (oopisGreaterThanOrEqualToandLessThan(savedFirstFields, oldSpaceStart,
                                           endOfMemory)) {
    freeObject(savedFirstFields);
  }
  freeObject(reachableObjectsArray);
  runLeakCheckerFor(GCModeImageSegment);
  assert(allObjectsUnmarked());
  return errCode;
}