/* Extracted from interp.c:34119 (function displayObject). */

/* SpurMemoryManager>>#displayObject */

sqInt displayObject(void) {
  return fetchPointerofObject(TheDisplay, specialObjectsOop);
}