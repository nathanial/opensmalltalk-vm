/* Extracted from interp.c:27411 (function isWordsNonImm). */

/*	Answer if the argument contains only indexable words (no oops). See
        comment in formatOf:
 */

/* Spur64BitMemoryManager>>#isWordsNonImm: */

static NoDbgRegParms int isWordsNonImm(sqInt objOop) {
  return ((((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
            (formatMask())) >= (firstLongFormat())) &&
          (((byteAt((void *)(objOop + (formatFieldByteOffset())))) &
            (formatMask())) <= ((firstShortFormat()) - 1)));
}