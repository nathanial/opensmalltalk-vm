/* Extracted from interp.c:33077 (function classCharacter). */

/* SpurMemoryManager>>#classCharacter */

sqInt classCharacter(void) {
  return longAt((void *)((specialObjectsOop + BaseHeaderSize) +
                         ((((usqInt)(ClassCharacter) << (shiftForWord()))))));
}