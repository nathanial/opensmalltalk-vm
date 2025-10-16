/* Extracted from interp.c:58280 (function penultimateLiteralOf). */

/* StackInterpreter>>#penultimateLiteralOf: */

static sqInt penultimateLiteralOf(sqInt aMethodOop) {
  sqInt offset;

  assert(isOopCompiledMethod(aMethodOop));
  offset = (literalCountOf(aMethodOop)) - 2;

  /* begin literal:ofMethod: */
  return fetchPointerofObject(offset + LiteralStart, aMethodOop);
}