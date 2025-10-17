/* Extracted from interp.c:17532 (function primitiveGreaterThanLargeIntegers).
 */

/*	Primitive comparison operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveGreaterThanLargeIntegers */

EXPORT(void)
primitiveGreaterThanLargeIntegers(void) {
  sqLong integerArg;
  sqLong integerRcvr;
  char *sp;

  integerArg = signed64BitValueOf(longAt(stackPointer));
  integerRcvr = signed64BitValueOf(longAt(stackPointer + (1 * BytesPerWord)));
  if (!primFailCode) {
    /* begin pop:thenPushBool: */
    longAtput((sp = stackPointer + (1 * BytesPerWord)), /* booleanObjectOf: */
              (integerRcvr > integerArg ? trueObj : falseObj));
    stackPointer = sp;
  }
}