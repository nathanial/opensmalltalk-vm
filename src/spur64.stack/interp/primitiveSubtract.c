/* Extracted from interp.c:24989 (function primitiveSubtract). */

/* InterpreterPrimitives>>#primitiveSubtract */

static void primitiveSubtract(void) {
  sqInt integerResult;
  char *sp;

  integerResult = (stackIntegerValue(1)) - (stackIntegerValue(0));

  /* begin pop2AndPushIntegerIfOK: */
  if (!primFailCode) {
    if ((((((usqInt)(integerResult)) >> 60) + 1) & 15) <= 1) {
      /* begin pop:thenPush: */
      longAtput((sp = stackPointer + (1 * BytesPerWord)),
                (((usqInt)integerResult << 3) | 1));
      stackPointer = sp;
    } else {
      /* begin success: */
      if (!primFailCode) {
        primFailCode = 1;
      }
    }
  }
}