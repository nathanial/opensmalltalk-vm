/* Extracted from interp.c:14770 (function primitiveDiv). */

/* InterpreterPrimitives>>#primitiveDiv */

static void primitiveDiv(void) {
  sqInt quotient;

  quotient = doPrimitiveDivby(longAt(stackPointer + (1 * BytesPerWord)),
                              longAt(stackPointer));

  /* begin pop2AndPushIntegerIfOK: */
  if (!primFailCode) {
    if ((((((usqInt)(quotient)) >> 60) + 1) & 15) <= 1) {
      popthenPush(2, (((usqInt)quotient << 3) | 1));
    } else {
      /* begin success: */
      if (!primFailCode) {
        primFailCode = 1;
      }
    }
  }
}