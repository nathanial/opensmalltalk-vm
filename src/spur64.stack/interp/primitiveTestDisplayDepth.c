/* Extracted from interp.c:25212 (function primitiveTestDisplayDepth). */

/*	Return true if the host OS does support the given display depth. */

/* InterpreterPrimitives>>#primitiveTestDisplayDepth */

static void primitiveTestDisplayDepth(void) {
  sqInt bitsPerPixel;
  sqInt integerPointer;
  sqInt okay;

  okay = 0;

  /* begin stackIntegerValue: */
  integerPointer = longAt(stackPointer);
  if ((((integerPointer) & 7) == 1)) {
    bitsPerPixel = (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    bitsPerPixel = 0;
  }
  if (!primFailCode) {
    okay = ioHasDisplayDepth(bitsPerPixel);
  }
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, /* booleanObjectOf: */
                    (okay ? trueObj : falseObj));
  }
}