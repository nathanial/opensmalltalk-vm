/* Extracted from interp.c:14857 (function primitiveDivideLargeIntegers). */

/*	Primitive arithmetic operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveDivideLargeIntegers */

EXPORT(void)
primitiveDivideLargeIntegers(void) {
  usqLong a;
  sqInt aIsNegative;
  usqLong b;
  sqInt bIsNegative;
  sqInt oopArg;
  sqInt oopRcvr;
  sqInt oopResult;
  usqLong result;

  oopArg = longAt(stackPointer);
  oopRcvr = longAt(stackPointer + (1 * BytesPerWord));
  aIsNegative = isNegativeIntegerValueOf(oopRcvr);
  bIsNegative = isNegativeIntegerValueOf(oopArg);
  a = magnitude64BitValueOf(oopRcvr);
  b = magnitude64BitValueOf(oopArg);
  if (primFailCode) {
    return;
  }

  /* check for exact division */
  if (!((b != 0) && ((a % b) == 0))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  result = a / b;
  oopResult = magnitude64BitIntegerForneg(result, aIsNegative != bIsNegative);
  if (!primFailCode) {
    popthenPush(2, oopResult);
  }
}