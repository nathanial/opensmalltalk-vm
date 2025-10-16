/* Extracted from interp.c:16120 (function primitiveFloatLessThan). */

/* InterpreterPrimitives>>#primitiveFloatLessThan */

static void primitiveFloatLessThan(void) {
  int aBool;

  aBool = primitiveFloatLessthanArg(longAt(stackPointer + (1 * BytesPerWord)),
                                    longAt(stackPointer));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, booleanObjectOf(aBool));
  }
}