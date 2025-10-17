/* Extracted from interp.c:18720 (function primitiveLessOrEqual). */

/* InterpreterPrimitives>>#primitiveLessOrEqual */

static void primitiveLessOrEqual(void) {
  sqInt integerArgument;
  sqInt integerReceiver;

  integerArgument = longAt(stackPointer);
  integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
  if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, /* booleanObjectOf: */
                    (integerReceiver <= integerArgument ? trueObj : falseObj));
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}