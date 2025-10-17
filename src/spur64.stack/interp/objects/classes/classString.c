/* Extracted from interp.c:33342 (function classString). */

/* SpurMemoryManager>>#classString */

sqInt classString(void) {
  return longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                         ((((usqInt)(ClassByteString) << (shiftForWord()))))));
}