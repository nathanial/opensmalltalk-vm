/* Extracted from interp.c:50125 (function argumentCountOf). */

/* StackInterpreter>>#argumentCountOf: */

usqInt argumentCountOf(sqInt methodPointer) {
  sqInt header;

  /* begin methodHeaderOf: */
  assert(isCompiledMethod(methodPointer));
  header = fetchPointerofObject(HeaderIndex, methodPointer);

  /* begin argumentCountOfMethodHeader: */
  return (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;
}