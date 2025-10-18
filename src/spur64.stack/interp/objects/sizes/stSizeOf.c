/* Extracted from interp.c:64934 (function stSizeOf). */

/*	Answer the number of indexable fields in the given object. (i.e., what
        Smalltalk would return for <obj> size).
 */
/*	Note: Assume oop is not an immediate! */

/* StackInterpreter>>#stSizeOf: */

sqInt stSizeOf(sqInt oop) {
  sqInt class;
  sqInt fixedFields;
  usqLong fmt;
  sqLong hdr;
  usqInt numSlots;
  sqInt totalLength;

  hdr = long64At((void *)(oop));
  fmt = (((usqLong)(hdr)) >> (formatShift())) & (formatMask());

  totalLength = lengthOfformat(oop, fmt);
l1:
  if (/* isPureBitsFormat: */
      (fmt >= (sixtyFourBitIndexableFormat())) &&
      (fmt < (firstCompiledMethodFormat()))) {
    return totalLength;
  }

  /* begin fixedFieldsOf:format:length: */
  if ((fmt >= (sixtyFourBitIndexableFormat())) || (fmt == (arrayFormat()))) {
    fixedFields = 0;
    goto l2;
  }
  if (fmt < (arrayFormat())) {
    fixedFields = totalLength;
    goto l2;
  }
  class = fetchClassOfNonImm(oop);
  fixedFields =
      (((fetchPointerofObject(InstanceSpecificationIndex, class)) >> 3)) &
      ((1U << (fixedFieldsFieldWidth())) - 1);
  /* end fixedFieldsOf:format:length: */
l2:
  if (fmt == (indexablePointersFormat())) {
    assert(!(isContextHeader(hdr)));
  }
  return totalLength - fixedFields;
}