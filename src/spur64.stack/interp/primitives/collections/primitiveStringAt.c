/* Extracted from interp.c:24235 (function primitiveStringAt). */

/* InterpreterPrimitives>>#primitiveStringAt */

static void primitiveStringAt(void) {
  sqInt class;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  sqInt index;
  usqInt numSlots;
  sqInt rcvr;
  sqInt result;
  char *sp;
  sqInt spSqInt;
  usqInt spUsqInt;
  sqInt stSize;
  sqInt totalLength;

  /* begin commonAt: */
  /* begin initPrimCall */
  primFailCode = 0;
  rcvr = longAt(stackPointer + (1 * BytesPerWord));
  index = longAt(stackPointer);
  if (((rcvr & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrInappropriate;
    goto l5;
  }

  /* No need to test for large positive integers here.  No object has 1g
   * elements */
  if (((!(index & (smallIntegerTag())))) ||
      ((argumentCount > 1) &&
       ((!((longAt((void *)(rcvr))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    goto l5;
  }
  index = (index >> 3);

  /* begin stObject:at: */
  hdr = long64At((void *)(rcvr));
  fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

  /* begin lengthOf:baseHeader:format: */
  /* begin lengthOf:format: */
  numSlots = numSlotsOfAny(rcvr);
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
      result = temporaryin(index - 1, frameOfMarriedContext(rcvr));
      goto l4;
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
    /* begin subscript:with:format: */
    if (fmt <= 5 /* lastPointerFormat */) {
      result = fetchPointerofObject((index + fixedFields) - 1, rcvr);
      goto l4;
    }
    if (fmt >= (firstByteFormat())) {
      result = (((usqInt)(byteAt((void *)((rcvr + BaseHeaderSize) +
                                          ((index + fixedFields) - 1))))
                 << 3) |
                1);
      goto l4;
    }
    if (fmt >= (firstShortFormat())) {
      result =
          (((usqInt)(((unsigned short)(shortAt(
                (void *)((rcvr + BaseHeaderSize) +
                         ((((usqInt)(((index + fixedFields) - 1)) << 1))))))))
            << 3) |
           1);
      goto l4;
    }
    if (fmt == (sixtyFourBitIndexableFormat())) {
      result = positive64BitIntegerFor(
          long64At((void *)((rcvr + BaseHeaderSize) +
                            ((((usqInt)(((index + fixedFields) - 1)) << 3))))));
      goto l4;
    }

    /* 32bit-word type objects; for now assume no 64-bit indexable objects */
    result =
        ((((((usqInt)(long32At(
               (void *)((rcvr + BaseHeaderSize) +
                        ((((usqInt)(((index + fixedFields) - 1)) << 2)))))))) &
           0xFFFFFFFFU)
          << 3) |
         1);
    goto l4;
  }

  /* primitiveFailFor: */
  primFailCode = (fmt <= 1 ? PrimErrBadReceiver : PrimErrBadIndex);
  result = 0;
  /* end stObject:at: */
l4:
  if (!primFailCode) {
    if (!(((result >= 0) && (result <= (0x3FFFFFFF))))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadReceiver;
      goto l5;
    }
    result =
        ((((usqInt)(((result >> 3))) << (numTagBits())))) + (characterTag());

    /* begin pop:thenPush: */
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              result);
    stackPointer = sp;
  }
  /* end commonAt: */
l5:;
}