/* Extracted from interp.c:37130 (function isArray). */

/*	Answer true if this is an indexable object with pointer elements, e.g.,
   an array
 */

/* SpurMemoryManager>>#isArray: */

sqInt isArray(sqInt oop) {
  return ((!(oop & (tagMask())))) &&
         (((byteAt((void *)(oop + (formatFieldByteOffset())))) &
           (formatMask())) == (arrayFormat()));
}