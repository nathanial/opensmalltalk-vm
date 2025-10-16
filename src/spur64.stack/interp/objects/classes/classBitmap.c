/* Extracted from interp.c:33058 (function classBitmap). */

/* SpurMemoryManager>>#classBitmap */

sqInt classBitmap(void) {
  return fetchPointerofObject(ClassBitmap, specialObjectsOop);
}