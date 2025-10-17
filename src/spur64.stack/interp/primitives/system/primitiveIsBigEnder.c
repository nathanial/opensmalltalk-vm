/* Extracted from interp.c:18680 (function primitiveIsBigEnder). */

/*	Answer if running on a big endian machine. */

/* InterpreterPrimitives>>#primitiveIsBigEnder */

EXPORT(sqInt)
primitiveIsBigEnder(void) {

  /* begin pop:thenPushBool: */
  popthenPushBool(1, falseObj);
  return 0;
}