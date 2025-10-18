/* Extracted from interp.c:64622 (function stObjectatput). */

/*	Do what ST would return for <obj> at: index put: value. */

/* StackInterpreter>>#stObject:at:put: */

sqInt stObjectatput(sqInt array, sqInt index, sqInt value) {
  sqInt class;
  sqInt fieldIndex;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  usqInt numSlots;
  sqInt signedValueToStore;
  usqInt sp;
  sqInt spSqInt;
  sqInt stSize;
  sqInt totalLength;
  usqLong unsigned64BitValueToStore;
  unsigned int unsignedValueToStore;

  hdr = long64At((void *)(array));
  fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

  /* begin lengthOf:baseHeader:format: */
  totalLength = lengthOfformat(array, fmt);
  /* end lengthOf:baseHeader:format: */
l1:

  /* begin fixedFieldsOf:format:length: */
  if ((fmt >= (sixtyFourBitIndexableFormat())) || (fmt == (arrayFormat()))) {
    fixedFields = 0;
    goto l2;
  }
  if (fmt < (arrayFormat())) {
    fixedFields = totalLength;
    goto l2;
  }
  class = fetchClassOfNonImm(array);
  fixedFields =
      (((fetchPointerofObject(InstanceSpecificationIndex, class)) >> 3)) &
      ((1U << (fixedFieldsFieldWidth())) - 1);
  /* end fixedFieldsOf:format:length: */
l2:
  if ((fmt == (indexablePointersFormat())) &&
      ((hdr & (classIndexMask())) == ClassMethodContextCompactIndex)) {
    /* begin stackPointerForMaybeMarriedContext: */
    if (/* isStillMarriedContext: */
        (((((fetchPointerofObject(SenderIndex, array))) & 7) == 1)) &&
        (!(isWidowedContext(array)))) {
      sp = stackPointerIndexForFrame(frameOfMarriedContext(array));
      assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(array)));
      stSize = sp;
      goto l3;
    }

    /* begin fetchStackPointerOf: */
    spSqInt = fetchPointerofObject(StackPointerIndex, array);
    if (!((((spSqInt) & 7) == 1))) {
      stSize = 0;
      goto l3;
    }
    assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(array)));
    stSize = (spSqInt >> 3);
    /* end stackPointerForMaybeMarriedContext: */
  l3:
    if ((oopisGreaterThanOrEqualTo(index, 1)) &&
        ((oopisLessThanOrEqualTo(index, stSize)) &&
         (/* isStillMarriedContext: */
          (((((fetchPointerofObject(SenderIndex, array))) & 7) == 1)) &&
          (!(isWidowedContext(array)))))) {
      return temporaryinput(index - 1, frameOfMarriedContext(array), value);
    }
  } else {
    stSize = totalLength - fixedFields;
  }
  if ((oopisGreaterThanOrEqualTo(
          index, /* firstValidIndexOfIndexableObject:withFormat: */
          (fmt >= (firstCompiledMethodFormat())
               ? (((literalCountOf(array)) + LiteralStart) * BytesPerOop) + 1
               : 1))) &&
      (oopisLessThanOrEqualTo(index, stSize))) {
    /* begin subscript:with:storing:format: */
    if (fmt <= 5 /* lastPointerFormat */) {
      fieldIndex = (index + fixedFields) - 1;

      storePointerofObjectwithValue(fieldIndex, array, value);
      goto l4;
    }
    if (fmt >= (firstByteFormat())) {
      if (!((((value) & 7) == 1))) {
        primFailCode = PrimErrBadArgument;
        goto l4;
      }
      signedValueToStore = (value >> 3);
      if (!((signedValueToStore >= 0) && (signedValueToStore <= 0xFF))) {
        primFailCode = PrimErrBadArgument;
        goto l4;
      }

      /* storeByte:ofObject:withValue: */
      byteAtput(
          (void *)((array + BaseHeaderSize) + ((index + fixedFields) - 1)),
          signedValueToStore);
      goto l4;
    }
    if (fmt >= (firstShortFormat())) {
      if (!((((value) & 7) == 1))) {
        primFailCode = PrimErrBadArgument;
        goto l4;
      }
      signedValueToStore = (value >> 3);
      if (!((signedValueToStore >= 0) && (signedValueToStore <= 0xFFFF))) {
        primFailCode = PrimErrBadArgument;
        goto l4;
      }

      /* storeShort16:ofObject:withValue: */
      shortAtput((void *)((array + BaseHeaderSize) +
                          ((((usqInt)(((index + fixedFields) - 1)) << 1)))),
                 signedValueToStore);
      goto l4;
    }
    if (fmt == (sixtyFourBitIndexableFormat())) {
      unsigned64BitValueToStore = positive64BitValueOf(value);
      if (!primFailCode) {
        /* storeLong64:ofObject:withValue: */
        long64Atput((void *)((array + BaseHeaderSize) +
                             ((((usqInt)(((index + fixedFields) - 1)) << 3)))),
                    unsigned64BitValueToStore);
      }
      goto l4;
    }

    /* 32bit-word type objects */
    unsignedValueToStore = positive32BitValueOf(value);
    if (!primFailCode) {
      /* storeLong32:ofObject:withValue: */
      long32Atput((void *)((array + BaseHeaderSize) +
                           ((((usqInt)(((index + fixedFields) - 1)) << 2)))),
                  unsignedValueToStore);
    }
    /* end subscript:with:storing:format: */
  l4:;
  } else {
    /* primitiveFailFor: */
    primFailCode = (fmt <= 1 ? PrimErrBadReceiver : PrimErrBadIndex);
  }
  return value;
}