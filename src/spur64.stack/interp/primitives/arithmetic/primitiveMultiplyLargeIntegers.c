/* Extracted from interp.c:19391 (function primitiveMultiplyLargeIntegers). */

/*	Primitive arithmetic operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveMultiplyLargeIntegers */

EXPORT(void)
primitiveMultiplyLargeIntegers(void) {
  usqLong a;
  sqInt aIsNegative;
  usqLong b;
  sqInt bIsNegative;
  sqInt oopArg;
  sqInt oopRcvr;
  sqInt oopResult;
  usqLong result;
  char *sp;

  oopArg = longAt(stackPointer);
  oopRcvr = longAt(stackPointer + (1 * BytesPerWord));
  aIsNegative = isNegativeIntegerValueOf(oopRcvr);
  bIsNegative = isNegativeIntegerValueOf(oopArg);
  a = magnitude64BitValueOf(oopRcvr);
  b = magnitude64BitValueOf(oopArg);
  if (primFailCode) {
    return;
  }

  /* check for overflow */
  if ((a > 1) && ((b > 1) && (a > (0xFFFFFFFFFFFFFFFFULL / b)))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  result = a * b;
  oopResult = magnitude64BitIntegerForneg(result, aIsNegative != bIsNegative);
  if (!primFailCode) {
    popthenPush(2, oopResult);
  }
}