/* Extracted from interp.c:19217 (function primitiveMod). */

/* InterpreterPrimitives>>#primitiveMod */

static void primitiveMod(void) {
  sqInt mod;
  char *sp;

  mod = doPrimitiveModby(longAt(stackPointer + (1 * BytesPerWord)),
                         longAt(stackPointer));

  /* begin pop2AndPushIntegerIfOK: */
  if (!primFailCode) {
    if ((((((usqInt)(mod)) >> 60) + 1) & 15) <= 1) {
      /* begin pop:thenPush: */
      popthenPush(2, (((usqInt)mod << 3) | 1));
    } else {
      /* begin success: */
      if (!primFailCode) {
        primFailCode = 1;
      }
    }
  }
}