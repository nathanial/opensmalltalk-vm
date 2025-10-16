/* Extracted from interp.c:42537 (function primitiveErrorTable). */

/* SpurMemoryManager>>#primitiveErrorTable */

sqInt primitiveErrorTable(void) {
  return fetchPointerofObject(PrimitiveErrorTableIndex, specialObjectsOop);
}