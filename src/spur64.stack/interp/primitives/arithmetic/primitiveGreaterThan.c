/* Extracted from interp.c:17503 (function primitiveGreaterThan). */

/* InterpreterPrimitives>>#primitiveGreaterThan */

static void primitiveGreaterThan(void) {
  sqInt integerArgument;
  sqInt integerReceiver;

  integerArgument = longAt(stackPointer);
  integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
  if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, booleanObjectOf(integerReceiver > integerArgument));
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}