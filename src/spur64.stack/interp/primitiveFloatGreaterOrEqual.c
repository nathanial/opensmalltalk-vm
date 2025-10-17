/* Extracted from interp.c:16066 (function primitiveFloatGreaterOrEqual). */

/* InterpreterPrimitives>>#primitiveFloatGreaterOrEqual */

static void primitiveFloatGreaterOrEqual(void) {
  int aBool;

  aBool = primitiveFloatGreaterOrEqualtoArg(
      longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, booleanObjectOf(aBool));
  }
}