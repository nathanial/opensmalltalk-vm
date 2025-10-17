/* Extracted from interp.c:52014 (function doPrimitiveModby). */

/* StackInterpreter>>#doPrimitiveMod:by: */

static sqInt doPrimitiveModby(sqInt rcvr, sqInt arg) {
  sqInt integerArg;
  sqInt integerRcvr;
  sqInt integerResult;

  integerArg = 0;
  integerRcvr = 0;
  if ((((rcvr & arg) & (smallIntegerTag())) != 0)) {
    integerRcvr = (rcvr >> 3);
    integerArg = (arg >> 3);

    /* begin success: */
    if (!integerArg) {
      if (!primFailCode) {
        primFailCode = 1;
      }
    }
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
  if (primFailCode) {
    return 1;
  }
  integerResult = integerRcvr % integerArg;

  /* ensure that the result has the same sign as the integerArg */
  if (integerArg < 0) {
    if (integerResult > 0) {
      integerResult += integerArg;
    }
  } else {
    if (integerResult < 0) {
      integerResult += integerArg;
    }
  }

  /* begin success: */
  if (!((((((usqInt)(integerResult)) >> 60) + 1) & 15) <= 1)) {
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
  return integerResult;
}