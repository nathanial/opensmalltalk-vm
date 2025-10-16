/* Extracted from interp.c:33342 (function classString). */

/* SpurMemoryManager>>#classString */

sqInt classString(void) {
  return fetchPointerofObject(ClassByteString, specialObjectsOop);
}