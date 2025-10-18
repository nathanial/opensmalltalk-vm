/* Extracted from interp.c:65111 (function tempCountOf). */

/* StackInterpreter>>#tempCountOf: */

usqInt tempCountOf(sqInt methodPointer) {
  sqInt header;

  header = methodHeaderOf(methodPointer);

  /* begin temporaryCountOfMethodHeader: */
  return (((usqInt)(header)) >> MethodHeaderTempCountShift) & 0x3F;
}