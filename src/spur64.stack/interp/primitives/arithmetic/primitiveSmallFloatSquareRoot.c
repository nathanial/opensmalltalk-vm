/* Extracted from interp.c:23826 (function primitiveSmallFloatSquareRoot). */

/* InterpreterPrimitives>>#primitiveSmallFloatSquareRoot */

static void primitiveSmallFloatSquareRoot(void) {
  sqInt aValue;
  usqLong bits;
  sqInt oop;
  double rcvr;

  oop = longAt(stackPointer);

  rcvr = smallFloatValueOf(oop);
  if (rcvr >= 0.0) {
    aValue = floatObjectOf(sqrt(rcvr));

    /* begin stackTopPut: */
    longAtput(stackPointer, aValue);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}