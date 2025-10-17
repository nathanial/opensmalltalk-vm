/* Extracted from interp.c:16048 (function primitiveFloatEqual). */

/* InterpreterPrimitives>>#primitiveFloatEqual */

static void primitiveFloatEqual(void) {
  int aBool;

  aBool = primitiveFloatEqualtoArg(longAt(stackPointer + (1 * BytesPerWord)),
                                   longAt(stackPointer));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, booleanObjectOf(aBool));
  }
}