/* Extracted from interp.c:24657 (function primitiveStringReplace). */

/*	<array> primReplaceFrom: start to: stop with: replacement startingAt:
        repStart
        <primitive: 105> */

/* InterpreterPrimitives>>#primitiveStringReplace */

static void primitiveStringReplace(void) {
  sqInt array;
  sqInt arrayFmt;
  sqInt arrayInstSize;
  usqInt arrayLength;
  sqInt class;
  sqInt i;
  sqInt mustRemember;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt oop;
  sqInt repl;
  sqInt replFmt;
  sqInt replInstSize;
  usqInt replLength;
  sqInt replStart;
  sqInt srcDelta;
  sqInt start;
  sqInt stop;
  sqInt valuePointer;

  /* begin primitiveSpurStringReplace */
  array = longAt(stackPointer + (4 * BytesPerWord));
  start = longAt(stackPointer + (3 * BytesPerWord));
  stop = longAt(stackPointer + (2 * BytesPerWord));
  repl = longAt(stackPointer + (1 * BytesPerWord));
  replStart = longAt(stackPointer);
  if (((!(start & (smallIntegerTag())))) ||
      (((!(stop & (smallIntegerTag())))) ||
       (((!(replStart & (smallIntegerTag())))) ||
        (((repl & (tagMask())) != 0))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    goto l7;
  }

  /* can happen in LgInt copy */
  start = (start >> 3);
  stop = (stop >> 3);
  replStart = (replStart >> 3);
  if ((stop >= start) &&
      (
#if IMMUTABILITY
          ((((usqInt)((
                byteAt((void *)(array + (immutableExtraBitsByteOffset())))))) >>
            (immutableBitByteShift())) &
           1) != 0
#else
          0
#endif
          )) {
    /* primitiveFailFor: */
    primFailCode = PrimErrNoModification;
    goto l7;
  }
  arrayFmt =
      (byteAt((void *)(array + (formatFieldByteOffset())))) & (formatMask());
  replFmt =
      (byteAt((void *)(repl + (formatFieldByteOffset())))) & (formatMask());

  /* N.B. In the below start - 1 to: stop - 1 do:, Slang is intelligent enough
   * to use < instead of <= so avoiding the stop - 1. */
  if (arrayFmt <= 5 /* lastPointerFormat */) {
    if (arrayFmt == (weakArrayFormat())) {
      arrayFmt = arrayFormat();
    }
    if (replFmt == (weakArrayFormat())) {
      replFmt = arrayFormat();
    }
    if (arrayFmt != replFmt) {
      /* primitiveFailFor: */
      primFailCode = PrimErrInappropriate;
      goto l7;
    }

    /* begin numSlotsOf: */
    assert((classIndexOf(array)) > (isForwardedObjectClassIndexPun()));
    arrayLength =
        (((numSlots = byteAt((void *)(array + (numSlotsFieldByteOffset()))))) ==
                 (numSlotsMask())
             ? ((((usqInt)((
                   (sqInt)((usqInt)((longAt((void *)(array - BaseHeaderSize))))
                           << 8)))))) >>
                   8
             : numSlots);

    /* begin fixedFieldsOf:format:length: */
    if ((arrayFmt >= (sixtyFourBitIndexableFormat())) ||
        (arrayFmt == (arrayFormat()))) {
      arrayInstSize = 0;
      goto l5;
    }
    if (arrayFmt < (arrayFormat())) {
      arrayInstSize = arrayLength;
      goto l5;
    }
    class = fetchClassOfNonImm(array);
    arrayInstSize = (((longAt((void *)((class + BaseHeaderSize) +
                                       ((((usqInt)(InstanceSpecificationIndex)
                                          << (shiftForWord()))))))) >>
                      3)) &
                    ((1U << (fixedFieldsFieldWidth())) - 1);
    /* end fixedFieldsOf:format:length: */
  l5:

    /* begin numSlotsOf: */
    assert((classIndexOf(repl)) > (isForwardedObjectClassIndexPun()));
    replLength =
        (((numSlots = byteAt((void *)(repl + (numSlotsFieldByteOffset()))))) ==
                 (numSlotsMask())
             ? ((((usqInt)((
                   (sqInt)((usqInt)((longAt((void *)(repl - BaseHeaderSize))))
                           << 8)))))) >>
                   8
             : numSlots);

    /* begin fixedFieldsOf:format:length: */
    if ((replFmt >= (sixtyFourBitIndexableFormat())) ||
        (replFmt == (arrayFormat()))) {
      replInstSize = 0;
      goto l6;
    }
    if (replFmt < (arrayFormat())) {
      replInstSize = replLength;
      goto l6;
    }
    class = fetchClassOfNonImm(repl);
    replInstSize = (((longAt((void *)((class + BaseHeaderSize) +
                                      ((((usqInt)(InstanceSpecificationIndex)
                                         << (shiftForWord()))))))) >>
                     3)) &
                   ((1U << (fixedFieldsFieldWidth())) - 1);
    /* end fixedFieldsOf:format:length: */
  l6:
    if (!((start >= 1) &&
          (((start - 1) <= stop) &&
           (((stop + arrayInstSize) <= arrayLength) &&
            ((replStart >= 1) && ((((stop - start) + replStart) +
                                   replInstSize) <= replLength)))))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadIndex;
      goto l7;
    }
    start += arrayInstSize;
    stop += arrayInstSize;
    srcDelta = (replStart + replInstSize) - start;

    /* begin isOldObject: */
    assert(isNonImmediate(array));
    if (oopisGreaterThanOrEqualTo(array, oldSpaceStart)) {
      mustRemember = 0;
      for (i = (start - 1); i < stop; i += 1) {
        oop = longAt(
            (void *)((repl + BaseHeaderSize) +
                     ((((usqInt)((srcDelta + i)) << (shiftForWord()))))));
        if (/* isYoung: */
            ((!(oop & (tagMask())))) && (oopisLessThan(oop, oldSpaceStart))) {
          mustRemember = 1;
        }

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(array)) && (!(isForwarded(array))));
        assert(validStorePointerUncheckedArgs(i, array, oop));
        longAtput((void *)((array + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  oop);
      }
      if (mustRemember) {
        /* begin possibleRootStoreInto: */
        if (!((byteAt((void *)(array + (formatFieldByteOffset())))) &
              (1U << (rememberedBitByteShift())))) {
          remember(array);
        }
      }
    } else {
      for (i = (start - 1); i < stop; i += 1) {
        valuePointer = longAt(
            (void *)((repl + BaseHeaderSize) +
                     ((((usqInt)((srcDelta + i)) << (shiftForWord()))))));

        /* begin storePointerUnchecked:ofObject:withValue: */
        assert((isNonImmediate(array)) && (!(isForwarded(array))));
        assert(validStorePointerUncheckedArgs(i, array, valuePointer));
        longAtput((void *)((array + BaseHeaderSize) +
                           ((((usqInt)(i) << (shiftForWord()))))),
                  valuePointer);
      }
    }

    /* We might consider comparing stop - start to some value here and using
     * forceInterruptCheck */

    /* begin pop: */
    stackPointer += argumentCount * BytesPerWord;
    goto l7;
  }

  /* Array formats must be the same; but for copying, weak arrays are equivalent
     to arrays. Non-pointer array formats must match */

  /* begin lengthOf:format: */
  /* begin numSlotsOfAny: */
  numSlotsUsqInt = byteAt((void *)(array + (numSlotsFieldByteOffset())));
  numSlots =
      (numSlotsUsqInt == (numSlotsMask())
           ? ((((usqInt)((
                 (sqInt)((usqInt)((longAt((void *)(array - BaseHeaderSize))))
                         << 8)))))) >>
                 8
           : numSlotsUsqInt);
  if (arrayFmt <= (ephemeronFormat())) {
    arrayLength = ((sqInt)numSlots);
    goto l3;
  }
  if (arrayFmt >= (firstByteFormat())) {
    arrayLength = ((numSlots << (shiftForWord()))) - (arrayFmt & 7);
    goto l3;
  }

  /* bytes, including CompiledMethod */
  if (arrayFmt >= (firstShortFormat())) {
    arrayLength = ((numSlots << ((shiftForWord()) - 1))) - (arrayFmt & 3);
    goto l3;
  }
  if (arrayFmt >= (firstLongFormat())) {
    arrayLength = ((numSlots << ((shiftForWord()) - 2))) - (arrayFmt & 1);
    goto l3;
  }
  if (arrayFmt == (sixtyFourBitIndexableFormat())) {
    arrayLength = ((sqInt)numSlots);
    goto l3;
  }

  /* fmt = self forwardedFormat */
  arrayLength = 0;
  /* end lengthOf:format: */
l3:

  /* begin lengthOf:format: */
  /* begin numSlotsOfAny: */
  numSlotsUsqInt = byteAt((void *)(repl + (numSlotsFieldByteOffset())));
  numSlots =
      (numSlotsUsqInt == (numSlotsMask())
           ? ((((usqInt)((
                 (sqInt)((usqInt)((longAt((void *)(repl - BaseHeaderSize))))
                         << 8)))))) >>
                 8
           : numSlotsUsqInt);
  if (replFmt <= (ephemeronFormat())) {
    replLength = ((sqInt)numSlots);
    goto l4;
  }
  if (replFmt >= (firstByteFormat())) {
    replLength = ((numSlots << (shiftForWord()))) - (replFmt & 7);
    goto l4;
  }

  /* bytes, including CompiledMethod */
  if (replFmt >= (firstShortFormat())) {
    replLength = ((numSlots << ((shiftForWord()) - 1))) - (replFmt & 3);
    goto l4;
  }
  if (replFmt >= (firstLongFormat())) {
    replLength = ((numSlots << ((shiftForWord()) - 2))) - (replFmt & 1);
    goto l4;
  }
  if (replFmt == (sixtyFourBitIndexableFormat())) {
    replLength = ((sqInt)numSlots);
    goto l4;
  }

  /* fmt = self forwardedFormat */
  replLength = 0;
  /* end lengthOf:format: */
l4:

  /* begin classFormatFromInstFormat: */
  if (arrayFmt >= (firstByteFormat())) {
    arrayFmt = arrayFmt & -8;
    goto l1;
  }

  /* this is likely the common case */
  if (arrayFmt <= (sixtyFourBitIndexableFormat())) {
    goto l1;
  }
  if (arrayFmt < (firstShortFormat())) {
    arrayFmt = arrayFmt & -2;
    goto l1;
  }
  arrayFmt = arrayFmt & -4;
  /* end classFormatFromInstFormat: */
l1:

  /* begin classFormatFromInstFormat: */
  if (replFmt >= (firstByteFormat())) {
    replFmt = replFmt & -8;
    goto l2;
  }

  /* this is likely the common case */
  if (replFmt <= (sixtyFourBitIndexableFormat())) {
    goto l2;
  }
  if (replFmt < (firstShortFormat())) {
    replFmt = replFmt & -2;
    goto l2;
  }
  replFmt = replFmt & -4;
  /* end classFormatFromInstFormat: */
l2:
  if (!((arrayFmt == replFmt) &&
        ((arrayFmt >= (sixtyFourBitIndexableFormat())) &&
         (arrayFmt < (firstCompiledMethodFormat()))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrInappropriate;
    goto l7;
  }
  if (!((start >= 1) && (((start - 1) <= stop) &&
                         ((stop <= arrayLength) &&
                          ((replStart >= 1) &&
                           (((stop - start) + replStart) <= replLength)))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    goto l7;
  }
  srcDelta = replStart - start;
  if (arrayFmt >= (firstShortFormat())) {
    if (arrayFmt >= (firstByteFormat())) {
      for (i = (start - 1); i < stop; i += 1) {
        /* storeByte:ofObject:withValue: */
        byteAtput((void *)((array + BaseHeaderSize) + i),
                  byteAt((void *)((repl + BaseHeaderSize) + (srcDelta + i))));
      }
    } else {
      for (i = (start - 1); i < stop; i += 1) {
        /* storeShort16:ofObject:withValue: */
        shortAtput((void *)((array + BaseHeaderSize) + ((((usqInt)(i) << 1)))),
                   shortAt((void *)((repl + BaseHeaderSize) +
                                    ((((usqInt)((srcDelta + i)) << 1))))));
      }
    }
  } else {
    if (arrayFmt >= (firstLongFormat())) {
      for (i = (start - 1); i < stop; i += 1) {
        /* storeLong32:ofObject:withValue: */
        long32Atput((void *)((array + BaseHeaderSize) + ((((usqInt)(i) << 2)))),
                    long32At((void *)((repl + BaseHeaderSize) +
                                      ((((usqInt)((srcDelta + i)) << 2))))));
      }
    } else {
      for (i = (start - 1); i < stop; i += 1) {
        /* storeLong64:ofObject:withValue: */
        long64Atput((void *)((array + BaseHeaderSize) + ((((usqInt)(i) << 3)))),
                    long64At((void *)((repl + BaseHeaderSize) +
                                      ((((usqInt)((srcDelta + i)) << 3))))));
      }
    }
  }

  /* 8 & 16-bit word type objects
     32 & 64-bit word type objects
     We might consider comparing stop - start to some value here and using
     forceInterruptCheck */

  /* begin pop: */
  stackPointer += argumentCount * BytesPerWord;
  /* end primitiveSpurStringReplace */
l7:;
}