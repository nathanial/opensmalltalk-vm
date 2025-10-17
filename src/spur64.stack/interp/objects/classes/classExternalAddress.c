/* Extracted from interp.c:33128 (function classExternalAddress). */

/* SpurMemoryManager>>#classExternalAddress */

sqInt classExternalAddress(void) {
  return longAt(
      (void *)((specialObjectsOop + BaseHeaderSize) +
               ((((usqInt)(ClassExternalAddress) << (shiftForWord()))))));
}