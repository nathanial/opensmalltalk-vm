/* Extracted from interp.c:27333 (function isImmediateFloat). */

/* Spur64BitMemoryManager>>#isImmediateFloat: */

static int isImmediateFloat(sqInt oop) {
  return ((oop & (smallFloatTag())) != 0);
}