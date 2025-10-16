/* Extracted from interp.c:14795 (function primitiveDivide). */

/* InterpreterPrimitives>>#primitiveDivide */

static void primitiveDivide(void) {
  sqInt integerArgument;
  sqInt integerPointer;
  sqInt integerReceiver;

  /* begin stackIntegerValue: */
  integerPointer = longAt(stackPointer + (1 * BytesPerWord));
  if ((((integerPointer) & 7) == 1)) {
    integerReceiver = (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    integerReceiver = 0;
  }

  /* begin stackIntegerValue: */
  integerPointer = longAt(stackPointer);
  if ((((integerPointer) & 7) == 1)) {
    integerArgument = (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    integerArgument = 0;
  }
  if ((integerArgument != 0) && ((integerReceiver % integerArgument) == 0)) {
    /* begin pop2AndPushIntegerIfOK: */
    if (!primFailCode) {
      if ((((((usqInt)((integerReceiver / integerArgument))) >> 60) + 1) &
           15) <= 1) {
        popthenPush(2,
                    (((usqInt)(integerReceiver / integerArgument) << 3) | 1));
      } else {
        /* begin success: */
        if (!primFailCode) {
          primFailCode = 1;
        }
      }
    }
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}