/* Extracted from interp.c:18749 (function primitiveLessOrEqualLargeIntegers).
 */

/*	Primitive comparison operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveLessOrEqualLargeIntegers */

EXPORT(void)
primitiveLessOrEqualLargeIntegers(void) {
  sqLong integerArg;
  sqLong integerRcvr;

  integerArg = signed64BitValueOf(longAt(stackPointer));
  integerRcvr = signed64BitValueOf(longAt(stackPointer + (1 * BytesPerWord)));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    popthenPushBool(2, /* booleanObjectOf: */
                    (integerRcvr <= integerArg ? trueObj : falseObj));
  }
}