/* Extracted from interp.c:19836 (function primitiveNotEqual). */

/* InterpreterPrimitives>>#primitiveNotEqual */

static void primitiveNotEqual(void) {
  sqInt integerArgument;
  sqInt integerReceiver;
  int result;

  integerArgument = longAt(stackPointer);
  integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
  if ((((integerReceiver & integerArgument) & (smallIntegerTag())) != 0)) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, /* booleanObjectOf: */
                    (integerReceiver != integerArgument ? trueObj : falseObj));
  } else {
    result = (signedMachineIntegerValueOf(integerReceiver)) !=
             (signedMachineIntegerValueOf(integerArgument));
    if (!primFailCode) {
      /* begin pop:thenPushBool: */
      popthenPushBool(2, /* booleanObjectOf: */
                      (result ? trueObj : falseObj));
    }
  }
}