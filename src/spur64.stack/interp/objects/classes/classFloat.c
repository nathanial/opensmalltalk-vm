/* Extracted from interp.c:33163 (function classFloat). */

/* SpurMemoryManager>>#classFloat */

sqInt classFloat(void) {
  return longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                         ((((usqInt)(ClassFloat) << (shiftForWord()))))));
}