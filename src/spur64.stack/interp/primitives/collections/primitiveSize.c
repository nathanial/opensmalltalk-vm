/* Extracted from interp.c:22644 (function primitiveSize). */

/* InterpreterPrimitives>>#primitiveSize */

static void primitiveSize(void) {
  sqInt class;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  usqInt numSlots;
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

  /* begin methodReturnInteger: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            (((usqInt)(totalLength - fixedFields) << 3) | 1));
  stackPointer = sp;
}