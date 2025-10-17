/* Extracted from interp.c:57726 (function methodClassOf). */

/*	Answer the method class of a method which is the value of an Association
        in the last literal,
        or answer nil if there isn't one.
        Using a read barrier here simplifies the become implementation and costs
        very little
        because the class index and ValueIndex of the association almost
   certainly share a cache line. */

/* StackInterpreter>>#methodClassOf: */

sqInt methodClassOf(sqInt methodPointer) {
  sqInt literal;
  sqInt offset;

  offset = (literalCountOf(methodPointer)) - 1;

  /* begin followLiteral:ofMethod: */
  /* begin followField:ofObject: */
  literal = fetchPointerofObject(offset + LiteralStart, methodPointer);
  if (isOopForwarded(literal)) {
    literal = fixFollowedFieldofObjectwithInitialValue(offset + LiteralStart,
                                                       methodPointer, literal);
  }
  return ((literal != nilObj) &&
                  (/* isPointers: */
                   ((!(literal & (tagMask())))) &&
                   (((byteAt((void *)(literal + (formatFieldByteOffset())))) &
                     (formatMask())) <= 5 /* lastPointerFormat */))
              ? (assert((numSlotsOf(literal)) > ValueIndex),
                 followFieldofObject(ValueIndex, literal))
              : nilObj);
}