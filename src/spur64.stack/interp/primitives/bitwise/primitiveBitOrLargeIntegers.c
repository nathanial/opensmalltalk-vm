/* Extracted from interp.c:12911 (function primitiveBitOrLargeIntegers). */

/*	Primitive logical operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveBitOrLargeIntegers */

EXPORT(void)
primitiveBitOrLargeIntegers(void) {
  usqLong integerArg;
  usqLong integerRcvr;
  sqInt oopResult;

  integerArg = positive64BitValueOf(longAt(stackPointer));
  integerRcvr = positive64BitValueOf(longAt(stackPointer + (1 * BytesPerWord)));
  if (primFailCode) {
    return;
  }
  oopResult = positive64BitIntegerFor(integerRcvr | integerArg);
  if (!primFailCode) {
    popthenPush(2, oopResult);
  }
}