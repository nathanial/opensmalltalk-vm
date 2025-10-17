/* Extracted from interp.c:18798 (function primitiveLessThanLargeIntegers). */

/*	Primitive comparison operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveLessThanLargeIntegers */

EXPORT(void)
primitiveLessThanLargeIntegers(void) {
  sqLong integerArg;
  sqLong integerRcvr;

  integerArg = signed64BitValueOf(longAt(stackPointer));
  integerRcvr = signed64BitValueOf(longAt(stackPointer + (1 * BytesPerWord)));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, booleanObjectOf(integerRcvr < integerArg));
  }
}