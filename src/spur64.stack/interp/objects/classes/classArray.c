/* Extracted from interp.c:32984 (function classArray). */

/* SpurMemoryManager>>#classArray */

sqInt classArray(void) {
  return fetchPointerofObject(ClassArray, specialObjectsOop);
}