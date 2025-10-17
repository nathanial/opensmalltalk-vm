/* Extracted from interp.c:33418 (function classUnsafeAlien). */

/* SpurMemoryManager>>#classUnsafeAlien */

sqInt classUnsafeAlien(void) {
  return longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                         ((((usqInt)(ClassUnsafeAlien) << (shiftForWord()))))));
}