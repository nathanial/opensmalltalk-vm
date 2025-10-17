/* Extracted from interp.c:50125 (function argumentCountOf). */

/* StackInterpreter>>#argumentCountOf: */

usqInt argumentCountOf(sqInt methodPointer) {
  sqInt header;

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(methodPointer));
  header = longAt((void *)((methodPointer + BaseHeaderSize) +
                           ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

  /* begin argumentCountOfMethodHeader: */
  return (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;
}