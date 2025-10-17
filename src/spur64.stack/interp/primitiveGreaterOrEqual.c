/* Extracted from interp.c:17454 (function primitiveGreaterOrEqual). */

/* InterpreterPrimitives>>#primitiveGreaterOrEqual */

static void primitiveGreaterOrEqual(void) {
  sqInt integerArgument;
  sqInt integerReceiver;

  integerArgument = longAt(stackPointer);
  integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
  if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, /* booleanObjectOf: */
                    (integerReceiver >= integerArgument ? trueObj : falseObj));
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}