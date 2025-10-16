/* Extracted from interp.c:37903 (function isWords). */

/*	Answer if the argument contains only indexable words (no oops). See
        comment in formatOf:
 */

/* SpurMemoryManager>>#isWords: */

sqInt isWords(sqInt oop) {
  return ((!(oop & (tagMask())))) &&
         (((((byteAt((void *)(oop + (formatFieldByteOffset())))) &
             (formatMask())) >= (firstLongFormat())) &&
           (((byteAt((void *)(oop + (formatFieldByteOffset())))) &
             (formatMask())) <= ((firstShortFormat()) - 1))));
}