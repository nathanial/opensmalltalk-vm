/* Extracted from interp.c:16084 (function primitiveFloatGreaterThan). */

/* InterpreterPrimitives>>#primitiveFloatGreaterThan */

static void primitiveFloatGreaterThan(void) {
  int aBool;

  aBool = primitiveFloatGreaterthanArg(
      longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, booleanObjectOf(aBool));
  }
}