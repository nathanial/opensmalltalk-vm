/* Extracted from interp.c:16240 (function primitiveFloatNotEqual). */

/* InterpreterPrimitives>>#primitiveFloatNotEqual */

static void primitiveFloatNotEqual(void) {
  int aBool;

  aBool = primitiveFloatEqualtoArg(longAt(stackPointer + (1 * BytesPerWord)),
                                   longAt(stackPointer));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, booleanObjectOf(!(aBool)));
  }
}