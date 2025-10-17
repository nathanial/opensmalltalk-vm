/* Extracted from interp.c:33058 (function classBitmap). */

/* SpurMemoryManager>>#classBitmap */

sqInt classBitmap(void) {
  return longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                         ((((usqInt)(ClassBitmap) << (shiftForWord()))))));
}