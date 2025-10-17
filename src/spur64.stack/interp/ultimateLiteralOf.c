/* Extracted from interp.c:65385 (function ultimateLiteralOf). */

/* StackInterpreter>>#ultimateLiteralOf: */

sqInt ultimateLiteralOf(sqInt aMethodOop) {
  sqInt offset;

  assert(isOopCompiledMethod(aMethodOop));
  offset = (literalCountOf(aMethodOop)) - 1;

  /* begin literal:ofMethod: */
  return longAt(
      (void *)((aMethodOop + BaseHeaderSize) +
               ((((usqInt)((offset + LiteralStart)) << (shiftForWord()))))));
}