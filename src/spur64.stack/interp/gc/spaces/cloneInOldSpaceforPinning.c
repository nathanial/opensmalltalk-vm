/* Extracted from interp.c:33566 (function cloneInOldSpaceforPinning). */

/* SpurMemoryManager>>#cloneInOldSpace:forPinning: */

static sqInt cloneInOldSpaceforPinning(sqInt objOop, sqInt forPinning) {
  sqInt fmt;
  sqInt hash;
  sqInt hasYoung;
  sqInt i;
  sqInt newObj;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt oop;
  sqInt referent;
  sqInt valuePointer;

  /* begin numSlotsOf: */
  assert((classIndexOf(objOop)) > (isForwardedObjectClassIndexPun()));
  numSlots =
      (((numSlotsUsqInt =
             byteAt((void *)(objOop + (numSlotsFieldByteOffset()))))) ==
               (numSlotsMask())
           ? ((((usqInt)((
                 (sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize))))
                         << 8)))))) >>
                 8
           : numSlotsUsqInt);
  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
  if (forPinning) {
    newObj = allocateSlotsForPinningInOldSpacebytesformatclassIndex(
        numSlots, /* objectBytesForSlots: */
        (numSlots ? ((numSlots << (shiftForWord()))) +
                        ((numSlots >= (numSlotsMask())
                              ? BaseHeaderSize + BaseHeaderSize
                              : BaseHeaderSize))
                  : 8 /* allocationUnit */ + BaseHeaderSize),
        fmt, (longAt((void *)(objOop))) & (classIndexMask()));
  } else {
    newObj = allocateSlotsInOldSpacebytesformatclassIndex(
        numSlots, /* objectBytesForSlots: */
        (numSlots ? ((numSlots << (shiftForWord()))) +
                        ((numSlots >= (numSlotsMask())
                              ? BaseHeaderSize + BaseHeaderSize
                              : BaseHeaderSize))
                  : 8 /* allocationUnit */ + BaseHeaderSize),
        fmt, (longAt((void *)(objOop))) & (classIndexMask()));
  }
  if (!newObj) {
    return 0;
  }
  if (fmt <= 5 /* lastPointerFormat */) {
    hasYoung = 0;
    for (i = 0; i < numSlots; i += 1) {
      oop = longAt((void *)((objOop + BaseHeaderSize) +
                            ((((usqInt)(i) << (shiftForWord()))))));
      if (((!(oop & (tagMask())))) &&
          ((!((longAt((void *)(oop))) &
              ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
        oop = followForwarded(oop);
      }
      if (((!(oop & (tagMask())))) &&
          ((/* begin isYoungObject: */
            assert(isNonImmediate(oop)), oopisLessThan(oop, oldSpaceStart)))) {
        hasYoung = 1;
      }

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newObj)) && (!(isForwarded(newObj))));
      assert(validStorePointerUncheckedArgs(i, newObj, oop));
      longAtput((void *)((newObj + BaseHeaderSize) +
                         ((((usqInt)(i) << (shiftForWord()))))),
                oop);
    }
    if (hasYoung) {
      remember(newObj);
    }
  } else {
    for (i = 0; i < numSlots; i += 1) {
      valuePointer = longAt((void *)((objOop + BaseHeaderSize) +
                                     ((((usqInt)(i) << (shiftForWord()))))));

      /* begin storePointerUnchecked:ofObject:withValue: */
      assert((isNonImmediate(newObj)) && (!(isForwarded(newObj))));
      assert(validStorePointerUncheckedArgs(i, newObj, valuePointer));
      longAtput((void *)((newObj + BaseHeaderSize) +
                         ((((usqInt)(i) << (shiftForWord()))))),
                valuePointer);
    }
    if (fmt >= (firstCompiledMethodFormat())) {
      if (((/* begin isYoungObject: */
            assert(isNonImmediate(objOop)),
            oopisLessThan(objOop, oldSpaceStart))) ||
          (((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
            (1U << (rememberedBitByteShift()))) != 0)) {
        remember(newObj);
      }
    }
  }
  if ((hash =
           (long32At((void *)(objOop + 4))) & (identityHashHalfWordMask()))) {
    /* begin setHashBitsOf:to: */
    long32Atput(
        (void *)(newObj + 4),
        ((((long32At((void *)(newObj + 4))) | (identityHashHalfWordMask())) -
          (identityHashHalfWordMask()))) +
            (hash & (identityHashHalfWordMask())));
  }
  if (
#if IMMUTABILITY
      ((((usqInt)((
            byteAt((void *)(objOop + (immutableExtraBitsByteOffset())))))) >>
        (immutableBitByteShift())) &
       1) != 0
#else
      0
#endif
  ) {
    /* begin setIsImmutableOf:to: */
    longAtput((void *)(newObj),
              (longAt((void *)(newObj))) | (1U << (immutableBitShift())));
  }
  return newObj;
}