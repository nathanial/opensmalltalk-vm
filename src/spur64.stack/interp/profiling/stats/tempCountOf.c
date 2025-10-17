/* Extracted from interp.c:65111 (function tempCountOf). */

/* StackInterpreter>>#tempCountOf: */

usqInt tempCountOf(sqInt methodPointer) {
  sqInt header;

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(methodPointer));
  header = fetchPointerofObject(HeaderIndex, methodPointer);

  /* begin temporaryCountOfMethodHeader: */
  return (((usqInt)(header)) >> MethodHeaderTempCountShift) & 0x3F;
}