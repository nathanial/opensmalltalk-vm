/* Extracted from interp.c:45109 (function
 * returnrestoringObjectsInsavedFirstFieldsandsavedHashes). */

/*	This is part of storeImageSegmentInto:outPointers:roots:. */

/* SpurMemoryManager>>#return:restoringObjectsIn:savedFirstFields:and:savedHashes:
 */

static sqInt returnrestoringObjectsInsavedFirstFieldsandsavedHashes(
    sqInt errCode, sqInt reachableObjectsArray, sqInt savedFirstFields,
    sqInt outPointersArray, sqInt savedHashes) {
  sqInt hash;
  sqInt i;
  usqInt numSlots;
  sqInt oop;
  sqInt toDoLimit;
  sqInt valuePointer;

  /* begin restoreObjectsIn:upTo:savedFirstFields: */

  assert((classIndexOf(reachableObjectsArray)) >
         (isForwardedObjectClassIndexPun()));
  numSlots = numSlotsOf(reachableObjectsArray);
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

  /* begin restoreObjectsIn:savedHashes: */
  toDoLimit = ((assert((classIndexOf(outPointersArray)) >
                       (isForwardedObjectClassIndexPun())),
                numSlotsOf(outPointersArray))) -
              1;
  for (i = 0; i <= toDoLimit; i += 1) {
    if (((hash = long32At((void *)((savedHashes + BaseHeaderSize) +
                                   ((((usqInt)(i) << 2))))))) >
        (identityHashHalfWordMask())) {
      if (oopisGreaterThanOrEqualToandLessThan(savedHashes, oldSpaceStart,
                                               endOfMemory)) {
        freeObject(savedHashes);
      }
      goto l1;
    }
    oop = fetchPointerofObject(i, outPointersArray);

    /* begin setHashBitsOf:to: */
    long32Atput((void *)(oop + 4), ((((long32At((void *)(oop + 4))) |
                                      (identityHashHalfWordMask())) -
                                     (identityHashHalfWordMask()))) +
                                       (hash & (identityHashHalfWordMask())));
    assert(!(isFreeObject(oop)));
    byteAtput((void *)(oop + (markBitsByteOffset())),
              (byteAt((void *)(oop + (markBitsByteOffset())))) &
                  (0xFF - (1U << (markedBitByteShift()))));
  }
  if (oopisGreaterThanOrEqualToandLessThan(savedHashes, oldSpaceStart,
                                           endOfMemory)) {
    freeObject(savedHashes);
  }
  /* end restoreObjectsIn:savedHashes: */
l1:
  freeObject(reachableObjectsArray);
  runLeakCheckerFor(GCModeImageSegment);
  assert(allObjectsUnmarked());
  return errCode;
}