/* Extracted from interp.c:32977 (function classAlien). */

/* SpurMemoryManager>>#classAlien */

sqInt classAlien(void) {
  return longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                         ((((usqInt)(ClassAlien) << (shiftForWord()))))));
}