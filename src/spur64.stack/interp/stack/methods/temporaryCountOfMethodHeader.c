/* Extracted from interp.c:65125 (function temporaryCountOfMethodHeader). */

/* StackInterpreter>>#temporaryCountOfMethodHeader: */

usqInt temporaryCountOfMethodHeader(sqInt header) {
  return (((usqInt)(header)) >> MethodHeaderTempCountShift) & 0x3F;
}