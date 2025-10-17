/* Extracted from interp.c:17483 (function
 * primitiveGreaterOrEqualLargeIntegers). */

/*	Primitive comparison operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveGreaterOrEqualLargeIntegers */

EXPORT(void)
primitiveGreaterOrEqualLargeIntegers(void) {
  sqLong integerArg;
  sqLong integerRcvr;

  integerArg = signed64BitValueOf(longAt(stackPointer));
  integerRcvr = signed64BitValueOf(longAt(stackPointer + (1 * BytesPerWord)));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, /* booleanObjectOf: */
                    (integerRcvr >= integerArg ? trueObj : falseObj));
  }
}