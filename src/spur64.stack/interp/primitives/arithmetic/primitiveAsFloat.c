/* Extracted from interp.c:12050 (function primitiveAsFloat). */

/*	N.B. This will answer inexact results for integers with > 53 bits of
        magnitude.
 */

/* InterpreterPrimitives>>#primitiveAsFloat */

static void primitiveAsFloat(void) {
  sqInt rcvr;

  rcvr = longAt(stackPointer);
  assert((((rcvr) & 7) == 1));

  /* begin pop:thenPushFloat: */
  popthenPushFloat(1, floatObjectOf(((double)((rcvr >> 3)))));
}
