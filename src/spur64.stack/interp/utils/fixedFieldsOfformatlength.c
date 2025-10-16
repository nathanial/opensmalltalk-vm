/* Extracted from interp.c:35449 (function fixedFieldsOfformatlength). */

/* SpurMemoryManager>>#fixedFieldsOf:format:length: */

static sqInt fixedFieldsOfformatlength(sqInt objOop, sqInt fmt,
                                       sqInt wordLength) {
  sqInt class;

  /* N.B. written to fall through to fetchClassOfNonImm: et al for forwarders
     so as to trigger an assert fail. */
  if ((fmt >= (sixtyFourBitIndexableFormat())) || (fmt == (arrayFormat()))) {
    return 0;
  }
  if (fmt < (arrayFormat())) {
    return wordLength;
  }
  class = fetchClassOfNonImm(objOop);
  return (((fetchPointerofObject(InstanceSpecificationIndex, class)) >> 3)) &
         ((1U << (fixedFieldsFieldWidth())) - 1);
}