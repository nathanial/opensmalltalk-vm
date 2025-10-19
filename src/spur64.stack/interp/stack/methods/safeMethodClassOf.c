/* Extracted from interp.c:62709 (function safeMethodClassOf). */

/*	Safe version of methodClassOf: that deals with malformed compiled
   methods, etc, and does not fixup forwarding pointers.. */

/* StackInterpreter>>#safeMethodClassOf: */

static sqInt safeMethodClassOf(sqInt methodPointer) {
  sqInt literal;
  sqInt maybeClass;
  sqInt offset;

  offset = (literalCountOf(methodPointer)) - 1;

  /* begin literal:ofMethod: */
  literal = fetchPointerofObject(offset + LiteralStart, methodPointer);
  if (isOopForwarded(literal)) {
    literal = followForwarded(literal);
  }
  if (!((/* isPointers: */
         ((!(literal & (tagMask())))) &&
         (((byteAt((void *)(literal + (formatFieldByteOffset())))) &
           (formatMask())) <= 5 /* lastPointerFormat */)) &&
        (((numSlotsOf(literal))) > ValueIndex))) {
    return nilObj;
  }
  maybeClass = fetchPointerofObject(ValueIndex, literal);
  if (isOopForwarded(maybeClass)) {
    maybeClass = followForwarded(maybeClass);
  }
  return maybeClass;
}
