/* Extracted from interp.c:66465 (function primitiveContextAt). */

/*	Special version of primitiveAt for accessing contexts.
        Written to be varargs for use from mirror primitives. */

/* StackInterpreterPrimitives>>#primitiveContextAt */

static void primitiveContextAt(void) {
  sqInt aContext;
  sqInt class;
  sqInt fixedFields;
  sqInt fixedFieldsSqInt;
  usqLong fmt;
  usqLong fmtUsqLong;
  usqInt frameNumArgs;
  sqLong hdr;
  sqLong hdrSqLong;
  sqInt index;
  usqInt numSlots;
  sqInt senderOop;
  char *sp;
  char *spouseFP;
  sqInt spSqInt;
  usqInt spUsqInt;
  sqInt stSize;
  sqInt stSizeSqInt;
  sqInt totalLength;
  sqInt totalLengthSqInt;
  sqInt value;

  index = longAt(stackPointer);
  if (!((((index) & 7) == 1))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  index = (index >> 3);
  aContext = longAt(stackPointer + (1 * BytesPerWord));

  /* Duplicating much of stObject:at:put: here allows stObject:at:put: to omit
   * tests for contexts. */
  hdr = long64At((void *)(aContext));
  if (!((hdr & (classIndexMask())) == ClassMethodContextCompactIndex)) {
    /* begin stObject:at: */
    hdrSqLong = long64At((void *)(aContext));
    fmtUsqLong = (((usqLong)(hdrSqLong)) >> (formatShift())) & (formatMask());

    totalLengthSqInt = lengthOfformat(aContext, fmtUsqLong);
  l7:

    /* begin fixedFieldsOf:format:length: */
    if ((fmtUsqLong >= (sixtyFourBitIndexableFormat())) ||
        (fmtUsqLong == (arrayFormat()))) {
      fixedFieldsSqInt = 0;
      goto l6;
    }
    if (fmtUsqLong < (arrayFormat())) {
      fixedFieldsSqInt = totalLengthSqInt;
      goto l6;
    }
    class = fetchClassOfNonImm(aContext);
    fixedFieldsSqInt =
        (((fetchPointerofObject(InstanceSpecificationIndex, class)) >> 3)) &
        ((1U << (fixedFieldsFieldWidth())) - 1);
    /* end fixedFieldsOf:format:length: */
  l6:
    if ((fmtUsqLong == (indexablePointersFormat())) &&
        ((hdrSqLong & (classIndexMask())) == ClassMethodContextCompactIndex)) {
      /* begin stackPointerForMaybeMarriedContext: */
      if (/* isStillMarriedContext: */
          (((((fetchPointerofObject(SenderIndex, aContext))) & 7) == 1)) &&
          (!(isWidowedContext(aContext)))) {
        spUsqInt = stackPointerIndexForFrame(frameOfMarriedContext(aContext));
        assert((ReceiverIndex + ((spUsqInt >> 3))) < (lengthOf(aContext)));
        stSizeSqInt = spUsqInt;
        goto l5;
      }

      /* begin fetchStackPointerOf: */
      spSqInt = fetchPointerofObject(StackPointerIndex, aContext);
      if (!((((spSqInt) & 7) == 1))) {
        stSizeSqInt = 0;
        goto l5;
      }
      assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(aContext)));
      stSizeSqInt = (spSqInt >> 3);
      /* end stackPointerForMaybeMarriedContext: */
    l5:
      if ((oopisGreaterThanOrEqualTo(index, 1)) &&
          ((oopisLessThanOrEqualTo(index, stSizeSqInt)) &&
           (/* isStillMarriedContext: */
            (((((longAt((
                   void *)((aContext + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
               7) == 1)) &&
            (!(isWidowedContext(aContext)))))) {
        value = temporaryin(index - 1, frameOfMarriedContext(aContext));
        goto l8;
      }
    } else {
      stSizeSqInt = totalLengthSqInt - fixedFieldsSqInt;
    }
    if ((oopisGreaterThanOrEqualTo(
            index, /* firstValidIndexOfIndexableObject:withFormat: */
            (fmtUsqLong >= (firstCompiledMethodFormat())
                 ? (((literalCountOf(aContext)) + LiteralStart) * BytesPerOop) +
                       1
                 : 1))) &&
        (oopisLessThanOrEqualTo(index, stSizeSqInt))) {
      /* begin subscript:with:format: */
      if (fmtUsqLong <= 5 /* lastPointerFormat */) {
        value = fetchPointerofObject((index + fixedFieldsSqInt) - 1, aContext);
        goto l8;
      }
      if (fmtUsqLong >= (firstByteFormat())) {
        value = (((usqInt)(byteAt((void *)((aContext + BaseHeaderSize) +
                                           ((index + fixedFieldsSqInt) - 1))))
                  << 3) |
                 1);
        goto l8;
      }
      if (fmtUsqLong >= (firstShortFormat())) {
        value = (((usqInt)(((unsigned short)(shortAt(
                      (void *)((aContext + BaseHeaderSize) +
                               ((((usqInt)(((index + fixedFieldsSqInt) - 1))
                                  << 1))))))))
                  << 3) |
                 1);
        goto l8;
      }
      if (fmtUsqLong == (sixtyFourBitIndexableFormat())) {
        value = positive64BitIntegerFor(long64At(
            (void *)((aContext + BaseHeaderSize) +
                     ((((usqInt)(((index + fixedFieldsSqInt) - 1)) << 3))))));
        goto l8;
      }

      /* 32bit-word type objects; for now assume no 64-bit indexable objects */
      value = ((((((usqInt)(long32At(
                     (void *)((aContext + BaseHeaderSize) +
                              ((((usqInt)(((index + fixedFieldsSqInt) - 1))
                                 << 2)))))))) &
                 0xFFFFFFFFU)
                << 3) |
               1);
      goto l8;
    }

    /* primitiveFailFor: */
    primFailCode = (fmtUsqLong <= 1 ? PrimErrBadReceiver : PrimErrBadIndex);
    value = 0;
    /* end stObject:at: */
  l8:
    if (!primFailCode) {
      /* begin pop:thenPush: */
      popthenPush(argumentCount + 1, value);
    }
    return;
  }

  /* might be an instance of a subclass */

  externalWriteBackHeadFramePointers();
  if (!(/* isStillMarriedContext: */
        (((((fetchPointerofObject(SenderIndex, aContext))) & 7) == 1)) &&
        (!(isWidowedContext(aContext))))) {
    fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

    totalLength = lengthOfformat(aContext, fmt);
  l4:

    /* begin fixedFieldsOf:format:length: */
    if ((fmt >= (sixtyFourBitIndexableFormat())) || (fmt == (arrayFormat()))) {
      fixedFields = 0;
      goto l3;
    }
    if (fmt < (arrayFormat())) {
      fixedFields = totalLength;
      goto l3;
    }
    class = fetchClassOfNonImm(aContext);
    fixedFields =
        (((fetchPointerofObject(InstanceSpecificationIndex, class)) >> 3)) &
        ((1U << (fixedFieldsFieldWidth())) - 1);
    /* end fixedFieldsOf:format:length: */
  l3:

    /* begin fetchStackPointerOf: */
    spSqInt = fetchPointerofObject(StackPointerIndex, aContext);
    if (!((((spSqInt) & 7) == 1))) {
      stSize = 0;
      goto l1;
    }
    assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(aContext)));
    stSize = (spSqInt >> 3);
    /* end fetchStackPointerOf: */
  l1:
    if (!(((index >= 1) && (index <= stSize)))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadIndex;
      return;
    }

    /* begin subscript:with:format: */
    if (fmt <= 5 /* lastPointerFormat */) {
      value = fetchPointerofObject((index + fixedFields) - 1, aContext);
      goto l2;
    }
    if (fmt >= (firstByteFormat())) {
      value = (((usqInt)(byteAt((void *)((aContext + BaseHeaderSize) +
                                         ((index + fixedFields) - 1))))
                << 3) |
               1);
      goto l2;
    }
    if (fmt >= (firstShortFormat())) {
      value =
          (((usqInt)(((unsigned short)(shortAt(
                (void *)((aContext + BaseHeaderSize) +
                         ((((usqInt)(((index + fixedFields) - 1)) << 1))))))))
            << 3) |
           1);
      goto l2;
    }
    if (fmt == (sixtyFourBitIndexableFormat())) {
      value = positive64BitIntegerFor(
          long64At((void *)((aContext + BaseHeaderSize) +
                            ((((usqInt)(((index + fixedFields) - 1)) << 3))))));
      goto l2;
    }

    /* 32bit-word type objects; for now assume no 64-bit indexable objects */
    value =
        ((((((usqInt)(long32At(
               (void *)((aContext + BaseHeaderSize) +
                        ((((usqInt)(((index + fixedFields) - 1)) << 2)))))))) &
           0xFFFFFFFFU)
          << 3) |
         1);
    /* end subscript:with:format: */
  l2:

    /* begin pop:thenPush: */
    popthenPush(argumentCount + 1, value);
    return;
  }

  /* begin frameOfMarriedContext: */
  senderOop = fetchPointerofObject(SenderIndex, aContext);
  assert((((senderOop) & 7) == 1));
  spouseFP = ((char *)(senderOop - (smallIntegerTag())));
  if (!(((index >= 1) && (index <= (stackPointerIndexForFrame(spouseFP)))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }
  value = temporaryin((index - 1), spouseFP);

  /* begin pop:thenPush: */
  popthenPush(argumentCount + 1, value);
}
