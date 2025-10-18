/* Extracted from interp.c:13000 (function primitiveBitShiftLargeIntegers). */

/*	Primitive arithmetic operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveBitShiftLargeIntegers */

EXPORT(void)
primitiveBitShiftLargeIntegers(void) {
  usqLong a;
  sqInt aIsNegative;
  sqInt integerPointer;
  sqInt oopRcvr;
  sqInt oopResult;
  usqLong result;
  sqInt shift;
  char *sp;

  /* begin stackIntegerValue: */
  integerPointer = longAt(stackPointer);
  if ((((integerPointer) & 7) == 1)) {
    shift = (integerPointer >> 3);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    shift = 0;
  }
  oopRcvr = longAt(stackPointer + (1 * BytesPerWord));
  aIsNegative = isNegativeIntegerValueOf(oopRcvr);
  a = magnitude64BitValueOf(oopRcvr);
  if (primFailCode) {
    return;
  }
  if (shift >= 0) {
    /* This is to avoid undue (usqInt) cast */
    result = 0xFFFFFFFFFFFFFFFFULL;
    if ((shift >= 64) || (a > ((result) >> shift))) {
      /* begin primitiveFail */
      if (!primFailCode) {
        primFailCode = 1;
      }
      return;
    }
    result = (a << shift);
  } else {
    shift = 0 - shift;
    if (shift >= 64) {
      result = 0;
    } else {
      result = (a) >> shift;
    }

    /* Fake 2 complement for negative values */
    if (aIsNegative && (((result << shift)) != a)) {
      result += 1;
    }
  }

  /* Protect against overflow */
  oopResult = magnitude64BitIntegerForneg(result, aIsNegative);
  if (!primFailCode) {
    /* begin pop:thenPush: */
    popthenPush(2, oopResult);
  }
}