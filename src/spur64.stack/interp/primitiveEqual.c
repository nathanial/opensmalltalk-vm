/* Extracted from interp.c:15035 (function primitiveEqual). */

/* InterpreterPrimitives>>#primitiveEqual */

static void primitiveEqual(void) {
  sqInt integerArgument;
  sqInt integerReceiver;
  int result;

  integerArgument = longAt(stackPointer);
  integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
  if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, /* booleanObjectOf: */
                    (integerReceiver == integerArgument ? trueObj : falseObj));
  } else {
    result = (signed64BitValueOf(integerReceiver)) ==
             (signed64BitValueOf(integerArgument));
    if (!primFailCode) {
      /* begin pop:thenPushBool: */
      popthenPushBool(2, /* booleanObjectOf: */
                      (result ? trueObj : falseObj));
    }
  }
}