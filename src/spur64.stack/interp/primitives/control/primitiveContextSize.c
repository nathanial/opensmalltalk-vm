/* Extracted from interp.c:67189 (function primitiveContextSize). */

/*	Special version of primitiveSize for accessing contexts.
        Written to be varargs for use from mirror primitives. */

/* StackInterpreterPrimitives>>#primitiveContextSize */

static void primitiveContextSize(void) {
  sqInt class;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  usqInt numSlots;
  sqInt rcvr;
  char *sp;
  sqInt spSqInt;
  usqInt spUsqInt;
  sqInt sz;
  sqInt totalLength;

  rcvr = longAt(stackPointer);
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
  if ((hdr & (classIndexMask())) == ClassMethodContextCompactIndex) {
    /* begin externalWriteBackHeadFramePointers */
    assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
    assert(stackPage == (mostRecentlyUsedPage));
    assert(!((isFree(stackPage))));

    /* begin setHeadFP:andSP:inPage: */
    assert(stackPointer < framePointer);
    assert((stackPointer < ((stackPage->baseAddress))) &&
           (stackPointer > (((stackPage->realStackLimit)) -
                            (LargeContextSlots * BytesPerOop))));
    assert((framePointer < ((stackPage->baseAddress))) &&
           (framePointer > (((stackPage->realStackLimit)) -
                            ((LargeContextSlots * BytesPerOop) / 2))));
    (stackPage->headFP = framePointer);
    (stackPage->headSP = stackPointer);
    assert(pageListIsWellFormed());

    /* begin stackPointerForMaybeMarriedContext: */
    if (/* isStillMarriedContext: */
        (((((fetchPointerofObject(SenderIndex, rcvr))) & 7) == 1)) &&
        (!(isWidowedContext(rcvr)))) {
      spUsqInt = stackPointerIndexForFrame(frameOfMarriedContext(rcvr));
      assert((ReceiverIndex + ((spUsqInt >> 3))) < (lengthOf(rcvr)));
      sz = spUsqInt;
      goto l3;
    }

    /* begin fetchStackPointerOf: */
    spSqInt = fetchPointerofObject(StackPointerIndex, rcvr);
    if (!((((spSqInt) & 7) == 1))) {
      sz = 0;
      goto l3;
    }
    assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(rcvr)));
    sz = (spSqInt >> 3);
    /* end stackPointerForMaybeMarriedContext: */
  l3:;
  } else {
    sz = totalLength - fixedFields;
  }

  /* begin methodReturnInteger: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            (((usqInt)sz << 3) | 1));
  stackPointer = sp;
}