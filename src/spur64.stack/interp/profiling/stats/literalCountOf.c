/* Extracted from interp.c:38215 (function literalCountOf). */

/* SpurMemoryManager>>#literalCountOf: */

usqInt literalCountOf(sqInt methodPointer) {
  sqInt header;

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(methodPointer));
  header = fetchPointerofObject(HeaderIndex, methodPointer);

  /* begin literalCountOfMethodHeader: */
  assert((((header) & 7) == 1));
  return ((header >> 3)) & AlternateHeaderNumLiteralsMask;
}