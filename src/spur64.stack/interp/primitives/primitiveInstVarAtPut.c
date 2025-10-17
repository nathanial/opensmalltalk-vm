/* Extracted from interp.c:68799 (function primitiveInstVarAtPut). */

/* StackInterpreterPrimitives>>#primitiveInstVarAtPut */

static void primitiveInstVarAtPut(void) {
  sqInt class;
  sqInt fieldIndex;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  sqInt index;
  sqInt newValue;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt rcvr;
  sqInt signedValueToStore;
  char *sp;
  sqInt totalLength;
  usqLong unsigned64BitValueToStore;
  unsigned int unsignedValueToStore;

  newValue = longAt(stackPointer);
  index = longAt(stackPointer + (1 * BytesPerWord));
  rcvr = longAt(stackPointer + (2 * BytesPerWord));
  if (((!(index & (smallIntegerTag())))) ||
      ((argumentCount > 2) && (isOopForwarded(rcvr)))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  if (((rcvr & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  if (
#if IMMUTABILITY
      ((((usqInt)((
            byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >>
        (immutableBitByteShift())) &
       1) != 0
#else
      0
#endif
  ) {
    /* primitiveFailFor: */
    primFailCode = PrimErrNoModification;
    return;
  }
  index = (index >> 3);
  hdr = long64At((void *)(rcvr));
  fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

  /* begin lengthOf:baseHeader:format: */
  /* begin lengthOf:format: */
  /* begin numSlotsOfAny: */
  numSlotsUsqInt = byteAt((void *)(rcvr + (numSlotsFieldByteOffset())));
  numSlots =
      (numSlotsUsqInt == (numSlotsMask())
           ? ((((usqInt)((
                 (sqInt)((usqInt)((longAt((void *)(rcvr - BaseHeaderSize))))
                         << 8)))))) >>
                 8
           : numSlotsUsqInt);
  if (fmt <= (ephemeronFormat())) {
    totalLength = numSlots;
    goto l2;
  }
  if (fmt >= (firstByteFormat())) {
    totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
    goto l2;
  }

  /* bytes, including CompiledMethod */
  if (fmt >= (firstShortFormat())) {
    totalLength = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
    goto l2;
  }
  if (fmt >= (firstLongFormat())) {
    totalLength = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
    goto l2;
  }
  if (fmt == (sixtyFourBitIndexableFormat())) {
    totalLength = numSlots;
    goto l2;
  }

  /* fmt = self forwardedFormat */
  totalLength = 0;
  /* end lengthOf:baseHeader:format: */
l2:

  /* begin fixedFieldsOf:format:length: */
  if ((fmt >= (sixtyFourBitIndexableFormat())) || (fmt == (arrayFormat()))) {
    fixedFields = 0;
    goto l1;
  }
  if (fmt < (arrayFormat())) {
    fixedFields = totalLength;
    goto l1;
  }
  class = fetchClassOfNonImm(rcvr);
  fixedFields =
      (((fetchPointerofObject(InstanceSpecificationIndex, class)) >> 3)) &
      ((1U << (fixedFieldsFieldWidth())) - 1);
  /* end fixedFieldsOf:format:length: */
l1:
  if (!((index >= 1) && (index <= fixedFields))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }
  if ((fmt == (indexablePointersFormat())) &&
      ((hdr & (classIndexMask())) == ClassMethodContextCompactIndex)) {
    externalInstVarofContextput(index - 1, rcvr, newValue);
  } else {
    /* begin subscript:with:storing:format: */
    if (fmt <= 5 /* lastPointerFormat */) {
      fieldIndex = index - 1;

      /* begin storePointer:ofObject:withValue: */
      assert(validStorePointerArgs(fieldIndex, rcvr, newValue));
      assert(isNonImmediate(rcvr));
      if (oopisGreaterThanOrEqualTo(rcvr, oldSpaceStart)) {
        if (/* isYoung: */
            ((!(newValue & (tagMask())))) &&
            (oopisLessThan(newValue, oldSpaceStart))) {
          /* begin possibleRootStoreInto: */
          if (!((byteAt((void *)(rcvr + (formatFieldByteOffset())))) &
                (1U << (rememberedBitByteShift())))) {
            remember(rcvr);
          }
        }
      }

      /* most stores into young objects */
      longAtput((void *)((rcvr + BaseHeaderSize) +
                         ((((usqInt)(fieldIndex) << (shiftForWord()))))),
                newValue);
      goto l3;
    }
    if (fmt >= (firstByteFormat())) {
      if (!((((newValue) & 7) == 1))) {
        primFailCode = PrimErrBadArgument;
        goto l3;
      }
      signedValueToStore = (newValue >> 3);
      if (!((signedValueToStore >= 0) && (signedValueToStore <= 0xFF))) {
        primFailCode = PrimErrBadArgument;
        goto l3;
      }

      /* storeByte:ofObject:withValue: */
      byteAtput((void *)((rcvr + BaseHeaderSize) + (index - 1)),
                signedValueToStore);
      goto l3;
    }
    if (fmt >= (firstShortFormat())) {
      if (!((((newValue) & 7) == 1))) {
        primFailCode = PrimErrBadArgument;
        goto l3;
      }
      signedValueToStore = (newValue >> 3);
      if (!((signedValueToStore >= 0) && (signedValueToStore <= 0xFFFF))) {
        primFailCode = PrimErrBadArgument;
        goto l3;
      }

      /* storeShort16:ofObject:withValue: */
      shortAtput(
          (void *)((rcvr + BaseHeaderSize) + ((((usqInt)((index - 1)) << 1)))),
          signedValueToStore);
      goto l3;
    }
    if (fmt == (sixtyFourBitIndexableFormat())) {
      unsigned64BitValueToStore = positive64BitValueOf(newValue);
      if (!primFailCode) {
        /* storeLong64:ofObject:withValue: */
        long64Atput((void *)((rcvr + BaseHeaderSize) +
                             ((((usqInt)((index - 1)) << 3)))),
                    unsigned64BitValueToStore);
      }
      goto l3;
    }

    /* 32bit-word type objects */
    unsignedValueToStore = positive32BitValueOf(newValue);
    if (!primFailCode) {
      /* storeLong32:ofObject:withValue: */
      long32Atput(
          (void *)((rcvr + BaseHeaderSize) + ((((usqInt)((index - 1)) << 2)))),
          unsignedValueToStore);
    }
    /* end subscript:with:storing:format: */
  l3:;
  }

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            newValue);
  stackPointer = sp;
}