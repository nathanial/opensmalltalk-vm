/* Extracted from interp.c:22644 (function primitiveSize). */

/* InterpreterPrimitives>>#primitiveSize */

static void primitiveSize(void) {
  sqInt class;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  usqInt numSlots;
  usqInt numSlotsUsqInt;
  sqInt rcvr;
  char *sp;
  sqInt totalLength;

  fmt = 0;
  rcvr = longAt(stackPointer);
  if ((((rcvr & (tagMask())) != 0)) ||
      (((hdr = long64At((void *)(rcvr))),
        (((fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask()))) <
         (arrayFormat())) ||
            (fmt == (forwardedFormat()))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }

  /* Integers are not indexable
     And if Spur, this is a forwarder. */
  if ((fmt == (indexablePointersFormat())) &&
      ((hdr & (classIndexMask())) == ClassMethodContextCompactIndex)) {
    primitiveContextSize();
    return;
  }

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
  fixedFields = (((fetchPointerofObject(InstanceSpecificationIndex, class)) >>
                  3)) &
                ((1U << (fixedFieldsFieldWidth())) - 1);
  /* end fixedFieldsOf:format:length: */
l1:

  /* begin methodReturnInteger: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            (((usqInt)(totalLength - fixedFields) << 3) | 1));
  stackPointer = sp;
}