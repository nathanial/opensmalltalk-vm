/* Extracted from interp.c:33675 (function cloneObject). */

/* SpurMemoryManager>>#cloneObject: */

sqInt cloneObject(sqInt objOop) {
  sqInt classIndex;
  sqInt fmt;
  sqInt hasYoung;
  sqInt i;
  sqInt newObj;
  usqInt newObjUsqInt;
  usqInt numBytes;
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
  if (numSlots > ((1U << (fixedFieldsFieldWidth())) - 1)) {
    classIndex = (longAt((void *)(objOop))) & (classIndexMask());

    /* begin allocateSlotsInOldSpace:format:classIndex: */
    newObj = allocateSlotsInOldSpacebytesformatclassIndex(
        numSlots, /* objectBytesForSlots: */
        (numSlots ? ((numSlots << (shiftForWord()))) +
                        ((numSlots >= (numSlotsMask())
                              ? BaseHeaderSize + BaseHeaderSize
                              : BaseHeaderSize))
                  : 8 /* allocationUnit */ + BaseHeaderSize),
        fmt, classIndex);
  } else {
    classIndex = (longAt((void *)(objOop))) & (classIndexMask());

    /* begin allocateSlots:format:classIndex: */
    if (numSlots >= (numSlotsMask())) {
      if (((numSlots) >> 56) > 0) {
        newObj = null;
        goto l1;
      }
      newObjUsqInt = freeStart + BaseHeaderSize;
      numBytes = (BaseHeaderSize + BaseHeaderSize) + (numSlots * BytesPerOop);
    } else {
      newObjUsqInt = freeStart;
      numBytes = BaseHeaderSize + ((numSlots < 1 ? 8 /* allocationUnit */
                                                 : numSlots * BytesPerOop));
    }
    if ((freeStart + numBytes) > scavengeThreshold) {
      if (numSlots <= ((1U << (fixedFieldsFieldWidth())) - 1)) {
        if (!needGCFlag) {
          /* begin scheduleScavenge */
          needGCFlag = 1;
          forceInterruptCheck();
        }
      }
      newObj = allocateSlotsInOldSpacebytesformatclassIndex(numSlots, numBytes,
                                                            fmt, classIndex);
      goto l1;
    }
    if (numSlots >= (numSlotsMask())) {
      longAtput((void *)(freeStart),
                ((((usqInt)((numSlotsMask())) << (numSlotsFullShift())))) +
                    numSlots);
      longAtput((void *)(newObjUsqInt),
                ((((((usqLong)(numSlotsMask()))) << (numSlotsFullShift()))) +
                 ((((usqInt)(fmt) << (formatShift()))))) +
                    classIndex);
    } else {
      longAtput((void *)(newObjUsqInt),
                ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
                 ((((usqInt)(fmt) << (formatShift()))))) +
                    classIndex);
    }

    /* for header parsing we put a saturated slot count in the prepended
     * overflow size word */
    assert((numBytes % (allocationUnit())) == 0);
    assert((newObjUsqInt % (allocationUnit())) == 0);
    freeStart += numBytes;
    newObj = newObjUsqInt;
    /* end allocateSlots:format:classIndex: */
  l1:;
  }
  if (newObj) {
    if (fmt <= 5 /* lastPointerFormat */) {
      hasYoung = 0;
      for (i = 0; i < numSlots; i += 1) {
        oop = longAt((void *)((objOop + BaseHeaderSize) +
                              ((((usqInt)(i) << (shiftForWord()))))));
        if ((!(oop & (tagMask())))) {
          if ((!((longAt((void *)(oop))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
            oop = followForwarded(oop);
          }
          if (((!(oop & (tagMask())))) &&
              ((/* begin isYoungObject: */
                assert(isNonImmediate(oop)),
                oopisLessThan(oop, oldSpaceStart)))) {
            hasYoung = 1;
          }
        }

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(newObj)) && (!(isForwarded(newObj))));
        assert(validStorePointerUncheckedArgs(i, newObj, oop));
        longAtput((void *)((newObj + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  oop);
      }
      if (hasYoung && (!((/* begin isYoungObject: */
                          assert(isNonImmediate(newObj)),
                          oopisLessThan(newObj, oldSpaceStart))))) {
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
        if (((/* begin isOldObject: */
              assert(isNonImmediate(newObj)),
              oopisGreaterThanOrEqualTo(newObj, oldSpaceStart))) &&
            (((/* begin isYoungObject: */
               assert(isNonImmediate(objOop)),
               oopisLessThan(objOop, oldSpaceStart))) ||
             (((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
               (1U << (rememberedBitByteShift()))) != 0))) {
          remember(newObj);
        }
      }
    }
  }
  return newObj;
}