/* Extracted from interp.c:37627 (function isShorts). */

/*	Answer if the argument contains only indexable 16-bit half words (no
        oops). See comment in formatOf:
 */

/* SpurMemoryManager>>#isShorts: */

sqInt isShorts(sqInt oop) {
  return ((!(oop & (tagMask())))) &&
         (((((byteAt((void *)(oop + (formatFieldByteOffset())))) &
             (formatMask())) >= (firstShortFormat())) &&
           (((byteAt((void *)(oop + (formatFieldByteOffset())))) &
             (formatMask())) <= ((firstByteFormat()) - 1))));
}