/* Extracted from interp.c:18769 (function primitiveLessThan). */

/* InterpreterPrimitives>>#primitiveLessThan */

static void primitiveLessThan(void) {
  sqInt integerArgument;
  sqInt integerReceiver;
  char *sp;

  integerArgument = longAt(stackPointer);
  integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
  if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
    /* begin pop:thenPushBool: */
    longAtput((sp = stackPointer + (1 * BytesPerWord)), /* booleanObjectOf: */
              (integerReceiver < integerArgument ? trueObj : falseObj));
    stackPointer = sp;
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}