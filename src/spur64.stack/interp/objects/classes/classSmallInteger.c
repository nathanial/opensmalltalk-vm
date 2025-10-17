/* Extracted from interp.c:33335 (function classSmallInteger). */

/* SpurMemoryManager>>#classSmallInteger */

sqInt classSmallInteger(void) {
  return longAt(
      (void *)((specialObjectsOop + BaseHeaderSize) +
               ((((usqInt)(ClassSmallInteger) << (shiftForWord()))))));
}