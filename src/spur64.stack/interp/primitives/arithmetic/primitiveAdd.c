/* Extracted from interp.c:11610 (function primitiveAdd). */

/* InterpreterPrimitives>>#primitiveAdd */

static void primitiveAdd(void) {
  sqInt integerResult;
  char *sp;

  integerResult = (stackIntegerValue(1)) + (stackIntegerValue(0));

  /* begin pop2AndPushIntegerIfOK: */
  if (!primFailCode) {
    if ((((((usqInt)(integerResult)) >> 60) + 1) & 15) <= 1) {
      popthenPush(2, (((usqInt)integerResult << 3) | 1));
    } else {
      /* begin success: */
      if (!primFailCode) {
        primFailCode = 1;
      }
    }
  }
}