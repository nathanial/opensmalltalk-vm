/* Extracted from interp.c:21416 (function primitiveRemLargeIntegers). */

/*	Primitive arithmetic operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveRemLargeIntegers */

EXPORT(void)
primitiveRemLargeIntegers(void) {
  usqLong a;
  sqInt aIsNegative;
  usqLong b;
  sqInt oopArg;
  sqInt oopRcvr;
  sqInt oopResult;
  usqLong result;

  oopArg = longAt(stackPointer);
  oopRcvr = longAt(stackPointer + (1 * BytesPerWord));
  aIsNegative = isNegativeIntegerValueOf(oopRcvr);
  a = magnitude64BitValueOf(oopRcvr);
  b = magnitude64BitValueOf(oopArg);
  if (!b) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
  if (primFailCode) {
    return;
  }
  result = a % b;
  oopResult = magnitude64BitIntegerForneg(result, aIsNegative);
  if (!primFailCode) {
    popthenPush(2, oopResult);
  }
}