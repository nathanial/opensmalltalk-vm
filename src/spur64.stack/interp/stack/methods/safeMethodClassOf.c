/* Extracted from interp.c:62709 (function safeMethodClassOf). */

/*	Safe version of methodClassOf: that deals with malformed compiled
   methods, etc, and does not fixup forwarding pointers.. */

/* StackInterpreter>>#safeMethodClassOf: */

static sqInt safeMethodClassOf(sqInt methodPointer) {
  sqInt literal;
  sqInt maybeClass;
  usqInt numSlots;
  sqInt offset;

  offset = (literalCountOf(methodPointer)) - 1;

  /* begin literal:ofMethod: */
  literal = longAt(
      (void *)((methodPointer + BaseHeaderSize) +
               ((((usqInt)((offset + LiteralStart)) << (shiftForWord()))))));
  if (/* isOopForwarded: */
      isOopForwarded(literal)) {
    literal = followForwarded(literal);
  }
  if (!((/* isPointers: */
         ((!(literal & (tagMask())))) &&
         (((byteAt((void *)(literal + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */)) &&
        (((/* begin numSlotsOf: */
           assert((classIndexOf(literal)) > (isForwardedObjectClassIndexPun())),
           (((numSlots =
                  byteAt((void *)(literal + (numSlotsFieldByteOffset()))))) ==
                    (numSlotsMask())
                ? ((((usqInt)(((sqInt)((usqInt)((longAt(
                                           (void *)(literal - BaseHeaderSize))))
                                       << 8)))))) >>
                      8
                : numSlots))) > ValueIndex))) {
    return nilObj;
  }
  maybeClass = longAt((void *)((literal + BaseHeaderSize) +
                               ((((usqInt)(ValueIndex) << (shiftForWord()))))));
  if (/* isOopForwarded: */
      isOopForwarded(maybeClass)) {
    maybeClass = followForwarded(maybeClass);
  }
  return maybeClass;
}