/* Extracted from interp.c:19245 (function primitiveModLargeIntegers). */

/*	Primitive arithmetic operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveModLargeIntegers */

EXPORT(void)
primitiveModLargeIntegers(void) {
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

  /* Handle remainder of same sign as argument */
  if (result) {
    if (!(bIsNegative == aIsNegative)) {
      result = b - result;
    }
  }
  oopResult = magnitude64BitIntegerForneg(result, bIsNegative);
  if (!primFailCode) {
    popthenPush(2, oopResult);
  }
}