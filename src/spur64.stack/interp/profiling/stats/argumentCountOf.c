/* Extracted from interp.c:50125 (function argumentCountOf). */

/* StackInterpreter>>#argumentCountOf: */

usqInt argumentCountOf(sqInt methodPointer) {
  sqInt header;

  header = methodHeaderOf(methodPointer);

  /* begin argumentCountOfMethodHeader: */
  return (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;
}