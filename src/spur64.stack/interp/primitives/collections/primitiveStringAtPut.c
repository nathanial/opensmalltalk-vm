/* Extracted from interp.c:24413 (function primitiveStringAtPut). */

/* InterpreterPrimitives>>#primitiveStringAtPut */

static void primitiveStringAtPut(void) {
  sqInt class;
  sqInt fieldIndex;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  sqInt index;
  usqInt numSlots;
  sqInt rcvr;
  sqInt signedValueToStore;
  char *sp;
  sqInt spSqInt;
  usqInt spUsqInt;
  sqInt stSize;
  sqInt totalLength;
  usqLong unsigned64BitValueToStore;
  unsigned int unsignedValueToStore;
  sqInt value;
  sqInt valueSqInt;

  /* begin commonAtPut: */
  /* begin initPrimCall */
  primFailCode = 0;
  rcvr = longAt(stackPointer + (2 * BytesPerWord));
  index = longAt(stackPointer + (1 * BytesPerWord));
  value = longAt(stackPointer);
  if (((rcvr & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrInappropriate;
    goto l11;
  }

  /* No need to test for large positive integers here.  No object has 1g
   * elements */
  if (((!(index & (smallIntegerTag())))) ||
      ((argumentCount > 2) &&
       ((!((longAt((void *)(rcvr))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    goto l11;
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
    goto l11;
  }
  index = (index >> 3);
  valueSqInt = asciiOfCharacter(value);

  /* begin stObject:at:put: */
  hdr = long64At((void *)(rcvr));
  fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

  /* begin lengthOf:baseHeader:format: */
  totalLength = lengthOfformat(rcvr, fmt);
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
  class = fetchClassOfNonImm(rcvr);
  fixedFields =
      (((fetchPointerofObject(InstanceSpecificationIndex, class)) >> 3)) &
      ((1U << (fixedFieldsFieldWidth())) - 1);
  /* end fixedFieldsOf:format:length: */
l2:
  if ((fmt == (indexablePointersFormat())) &&
      ((hdr & (classIndexMask())) == ClassMethodContextCompactIndex)) {
    /* begin stackPointerForMaybeMarriedContext: */
    if (/* isStillMarriedContext: */
        (((((fetchPointerofObject(SenderIndex, rcvr))) & 7) == 1)) &&
        (!(isWidowedContext(rcvr)))) {
      spUsqInt = stackPointerIndexForFrame(frameOfMarriedContext(rcvr));
      assert((ReceiverIndex + ((spUsqInt >> 3))) < (lengthOf(rcvr)));
      stSize = spUsqInt;
      goto l3;
    }

    /* begin fetchStackPointerOf: */
    spSqInt = fetchPointerofObject(StackPointerIndex, rcvr);
    if (!((((spSqInt) & 7) == 1))) {
      stSize = 0;
      goto l3;
    }
    assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(rcvr)));
    stSize = (spSqInt >> 3);
    /* end stackPointerForMaybeMarriedContext: */
  l3:
    if ((oopisGreaterThanOrEqualTo(index, 1)) &&
        ((oopisLessThanOrEqualTo(index, stSize)) &&
         (/* isStillMarriedContext: */
          (((((fetchPointerofObject(SenderIndex, rcvr))) & 7) == 1)) &&
          (!(isWidowedContext(rcvr)))))) {
      temporaryinput(index - 1, frameOfMarriedContext(rcvr), valueSqInt);
      goto l5;
    }
  } else {
    stSize = totalLength - fixedFields;
  }
  if ((oopisGreaterThanOrEqualTo(
          index, /* firstValidIndexOfIndexableObject:withFormat: */
          (fmt >= (firstCompiledMethodFormat())
               ? (((literalCountOf(rcvr)) + LiteralStart) * BytesPerOop) + 1
               : 1))) &&
      (oopisLessThanOrEqualTo(index, stSize))) {
    /* begin subscript:with:storing:format: */
    if (fmt <= 5 /* lastPointerFormat */) {
      fieldIndex = (index + fixedFields) - 1;

      storePointerofObjectwithValue(fieldIndex, rcvr, valueSqInt);
      goto l4;
    }
    if (fmt >= (firstByteFormat())) {
      if (!((((valueSqInt) & 7) == 1))) {
        primFailCode = PrimErrBadArgument;
        goto l4;
      }
      signedValueToStore = (valueSqInt >> 3);
      if (!((signedValueToStore >= 0) && (signedValueToStore <= 0xFF))) {
        primFailCode = PrimErrBadArgument;
        goto l4;
      }

      /* storeByte:ofObject:withValue: */
      byteAtput((void *)((rcvr + BaseHeaderSize) + ((index + fixedFields) - 1)),
                signedValueToStore);
      goto l4;
    }
    if (fmt >= (firstShortFormat())) {
      if (!((((valueSqInt) & 7) == 1))) {
        primFailCode = PrimErrBadArgument;
        goto l4;
      }
      signedValueToStore = (valueSqInt >> 3);
      if (!((signedValueToStore >= 0) && (signedValueToStore <= 0xFFFF))) {
        primFailCode = PrimErrBadArgument;
        goto l4;
      }

      /* storeShort16:ofObject:withValue: */
      shortAtput((void *)((rcvr + BaseHeaderSize) +
                          ((((usqInt)(((index + fixedFields) - 1)) << 1)))),
                 signedValueToStore);
      goto l4;
    }
    if (fmt == (sixtyFourBitIndexableFormat())) {
      unsigned64BitValueToStore = positive64BitValueOf(valueSqInt);
      if (!primFailCode) {
        /* storeLong64:ofObject:withValue: */
        long64Atput((void *)((rcvr + BaseHeaderSize) +
                             ((((usqInt)(((index + fixedFields) - 1)) << 3)))),
                    unsigned64BitValueToStore);
      }
      goto l4;
    }

    /* 32bit-word type objects */
    unsignedValueToStore = positive32BitValueOf(valueSqInt);
    if (!primFailCode) {
      /* storeLong32:ofObject:withValue: */
      long32Atput((void *)((rcvr + BaseHeaderSize) +
                           ((((usqInt)(((index + fixedFields) - 1)) << 2)))),
                  unsignedValueToStore);
    }
    /* end subscript:with:storing:format: */
  l4:;
  } else {
    /* primitiveFailFor: */
    primFailCode = (fmt <= 1 ? PrimErrBadReceiver : PrimErrBadIndex);
  }
  /* end stObject:at:put: */
l5:
  if (!primFailCode) {
    /* begin pop:thenPush: */
    popthenPush(argumentCount + 1, value);
  }
  /* end commonAtPut: */
l11:;
}