/* Extracted from interp.c:33156 (function classExternalStructure). */

/* SpurMemoryManager>>#classExternalStructure */

sqInt classExternalStructure(void) {
  return longAt(
      (void *)((specialObjectsOop + BaseHeaderSize) +
               ((((usqInt)(ClassExternalStructure) << (shiftForWord()))))));
}