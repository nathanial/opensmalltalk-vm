/* Extracted from interp.c:33135 (function classExternalData). */

/* SpurMemoryManager>>#classExternalData */

sqInt classExternalData(void) {
  return longAt(
      (void *)((specialObjectsOop + BaseHeaderSize) +
               ((((usqInt)(ClassExternalData) << (shiftForWord()))))));
}