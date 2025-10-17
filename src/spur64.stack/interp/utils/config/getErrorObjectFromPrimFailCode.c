/* Extracted from interp.c:54199 (function getErrorObjectFromPrimFailCode). */

/*	Answer the errorCode object to supply to a failing primitive method that
        accepts one.
        If there is a primitive error table and the primFailCode is a valid
   index there-in answer the corresponding entry in the table, otherwise simply
   answer the code as an integer.
 */

/* StackInterpreter>>#getErrorObjectFromPrimFailCode */

static sqInt getErrorObjectFromPrimFailCode(void) {
  sqInt classIndex;
  sqInt clone;
  sqInt errObj;
  sqInt i;
  usqInt newObj;
  usqInt numBytes;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt table;
  sqInt valuePointer;

  if (primFailCode > 0) {
    table = fetchPointerofObject(PrimitiveErrorTableIndex, specialObjectsOop);
    if (primFailCode <=
        ((/* begin numSlotsOf: */
          assert((classIndexOf(table)) > (isForwardedObjectClassIndexPun())),
          (((numSlots =
                 byteAt((void *)(table + (numSlotsFieldByteOffset()))))) ==
                   (numSlotsMask())
               ? ((((usqInt)(((
                     sqInt)((usqInt)((longAt((void *)(table - BaseHeaderSize))))
                            << 8)))))) >>
                     8
               : numSlots)))) {
      /* begin followField:ofObject: */
      errObj = fetchPointerofObject(primFailCode - 1, table);
      if (isOopForwarded(errObj)) {
        errObj = fixFollowedFieldofObjectwithInitialValue(primFailCode - 1,
                                                          table, errObj);
      }

      /* If there's a clonable object in the table at that index,
         answer a clone of the error object with the second slot set to the
         value of secondaryErrorCode. */
      if (((byteAt((void *)(errObj + (formatFieldByteOffset())))) &
           (formatMask())) == (nonIndexablePointerFormat())) {
        /* begin cloneErrorObj: */
        /* begin numSlotsOf: */
        assert((classIndexOf(errObj)) > (isForwardedObjectClassIndexPun()));
        numSlots =
            (((numSlotsUsqInt =
                   byteAt((void *)(errObj + (numSlotsFieldByteOffset()))))) ==
                     (numSlotsMask())
                 ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                            (void *)(errObj - BaseHeaderSize))))
                                        << 8)))))) >>
                       8
                 : numSlotsUsqInt);
        classIndex = (longAt((void *)(errObj))) & (classIndexMask());

        /* begin eeInstantiateAnySmallClassIndex:format:numSlots: */
        assert((numSlots >= 0) &&
               ((classIndex != 0) && ((classAtIndex(classIndex)) != nilObj)));
        assert((nonIndexablePointerFormat()) ==
               (instSpecOfClass(classAtIndex(classIndex))));

        /* begin allocateSmallNewSpaceSlots:format:classIndex: */
        assert(numSlots < (numSlotsMask()));
        newObj = freeStart;
        numBytes = BaseHeaderSize + ((numSlots < 1 ? 8 /* allocationUnit */
                                                   : numSlots * BytesPerOop));
        assert((numBytes % (allocationUnit())) == 0);
        assert((newObj % (allocationUnit())) == 0);
        if ((freeStart + numBytes) > scavengeThreshold) {
          if (!needGCFlag) {
            /* begin scheduleScavenge */
            needGCFlag = 1;
            forceInterruptCheck();
          }
          if ((freeStart + numBytes) > (((eden).limit))) {
            error("no room in eden for "
                  "allocateSmallNewSpaceSlots:format:classIndex:");
            clone = 0;
            goto l1;
          }
        }
        long64Atput(
            (void *)(newObj),
            ((((((usqLong)numSlots)) << (numSlotsFullShift()))) +
             ((((usqInt)((nonIndexablePointerFormat())) << (formatShift()))))) +
                classIndex);
        freeStart += numBytes;
        clone = newObj;
        /* end eeInstantiateAnySmallClassIndex:format:numSlots: */
      l1:
        for (i = 0; i < numSlots; i += 1) {
          valuePointer = fetchPointerofObject(i, errObj);

          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(clone)) && (!(isForwarded(clone))));
          assert(validStorePointerUncheckedArgs(i, clone, valuePointer));
          longAtput((void *)((clone + BaseHeaderSize) +
                             ((((usqInt)(i) << (shiftForWord()))))),
                    valuePointer);
        }
        if ((numSlots > 2) && (primFailCode == PrimErrFFIException)) {
          valuePointer = positive64BitIntegerFor(((usqLong)secondaryErrorCode));

          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(clone)) && (!(isForwarded(clone))));
          assert(validStorePointerUncheckedArgs(1, clone, valuePointer));
          longAtput(
              (void *)((clone + BaseHeaderSize) + (1U << (shiftForWord()))),
              valuePointer);
          valuePointer = positive64BitIntegerFor(exceptionPC);

          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(clone)) && (!(isForwarded(clone))));
          assert(validStorePointerUncheckedArgs(2, clone, valuePointer));
          longAtput(
              (void *)((clone + BaseHeaderSize) + (2U << (shiftForWord()))),
              valuePointer);
        } else {
          valuePointer = signed64BitIntegerFor(secondaryErrorCode);

          /* begin storePointerUnchecked:ofObject:withValue: */
          assert((isNonImmediate(clone)) && (!(isForwarded(clone))));
          assert(validStorePointerUncheckedArgs(1, clone, valuePointer));
          longAtput(
              (void *)((clone + BaseHeaderSize) + (1U << (shiftForWord()))),
              valuePointer);
        }
        errObj = clone;
      }
      return errObj;
    }
  }
  return (((usqInt)primFailCode << 3) | 1);
}