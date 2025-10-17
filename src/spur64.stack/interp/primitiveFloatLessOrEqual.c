/* Extracted from interp.c:16102 (function primitiveFloatLessOrEqual). */

/* InterpreterPrimitives>>#primitiveFloatLessOrEqual */

static void primitiveFloatLessOrEqual(void) {
  int aBool;

  aBool = primitiveFloatLessOrEqualtoArg(
      longAt(stackPointer + (1 * BytesPerWord)), longAt(stackPointer));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, /* booleanObjectOf: */
                    (aBool ? trueObj : falseObj));
  }
}