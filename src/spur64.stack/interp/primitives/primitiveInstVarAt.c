/* Extracted from interp.c:68674 (function primitiveInstVarAt). */

/*	because of externalInstVar:ofContext: below */

/* StackInterpreterPrimitives>>#primitiveInstVarAt */

static void primitiveInstVarAt(void) {
  sqInt class;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  sqInt index;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt rcvr;
  char *sp;
  sqInt totalLength;
  sqInt value;

  index = longAt(stackPointer);
  rcvr = longAt(stackPointer + (1 * BytesPerWord));
  if (((!(index & (smallIntegerTag())))) ||
      ((argumentCount > 1) && (isOopForwarded(rcvr)))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  if (((rcvr & (tagMask())) != 0)) {
    /* primitiveFailFor: */
    primFailCode = PrimErrInappropriate;
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
    goto l3;
  }
  if (fmt >= (firstByteFormat())) {
    totalLength = ((numSlots << (shiftForWord()))) - (fmt & 7);
    goto l3;
  }

  /* bytes, including CompiledMethod */
  if (fmt >= (firstShortFormat())) {
    totalLength = ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
    goto l3;
  }
  if (fmt >= (firstLongFormat())) {
    totalLength = ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
    goto l3;
  }
  if (fmt == (sixtyFourBitIndexableFormat())) {
    totalLength = numSlots;
    goto l3;
  }

  /* fmt = self forwardedFormat */
  totalLength = 0;
  /* end lengthOf:baseHeader:format: */
l3:

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
  if (!((index >= 1) && (index <= fixedFields))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadIndex;
    return;
  }
  if ((fmt == (indexablePointersFormat())) &&
      ((hdr & (classIndexMask())) == ClassMethodContextCompactIndex)) {
    value = externalInstVarofContext(index - 1, rcvr);
  } else {
    /* begin subscript:with:format: */
    if (fmt <= 5 /* lastPointerFormat */) {
      value = fetchPointerofObject(index - 1, rcvr);
      goto l1;
    }
    if (fmt >= (firstByteFormat())) {
      value =
          (((usqInt)(byteAt((void *)((rcvr + BaseHeaderSize) + (index - 1))))
            << 3) |
           1);
      goto l1;
    }
    if (fmt >= (firstShortFormat())) {
      value = (((usqInt)(((unsigned short)(shortAt(
                    (void *)((rcvr + BaseHeaderSize) +
                             ((((usqInt)((index - 1)) << 1))))))))
                << 3) |
               1);
      goto l1;
    }
    if (fmt == (sixtyFourBitIndexableFormat())) {
      value = positive64BitIntegerFor(long64At((
          void *)((rcvr + BaseHeaderSize) + ((((usqInt)((index - 1)) << 3))))));
      goto l1;
    }

    /* 32bit-word type objects; for now assume no 64-bit indexable objects */
    value =
        ((((((usqInt)(long32At((void *)((rcvr + BaseHeaderSize) +
                                        ((((usqInt)((index - 1)) << 2)))))))) &
           0xFFFFFFFFU)
          << 3) |
         1);
    /* end subscript:with:format: */
  l1:;
  }

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            value);
  stackPointer = sp;
}