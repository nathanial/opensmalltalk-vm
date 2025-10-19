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

  totalLength = lengthOfformat(rcvr, fmt);
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

      storePointerofObjectwithValue(fieldIndex, rcvr, newValue);
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

  popthenPush(argumentCount + 1, newValue);
}