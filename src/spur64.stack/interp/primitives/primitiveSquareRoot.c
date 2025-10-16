/* Extracted from interp.c:24110 (function primitiveSquareRoot). */

/*	Computes square root of float receiver; receiver *must* be a float
        instance.
 */

/* InterpreterPrimitives>>#primitiveSquareRoot */

static void primitiveSquareRoot(void) {
  sqInt aValue;
  double doubleValue;
  sqInt rcvr;

  rcvr = longAt(stackPointer);

  /* begin noFailFloatValueOf: */
  assert(isFloatInstance(rcvr));
  if (rcvr & (tagMask())) {
    doubleValue = smallFloatValueOf(rcvr);
  } else {
    fetchFloatAtinto(rcvr + BaseHeaderSize, doubleValue);
  }
  if (doubleValue >= 0.0) {
    aValue = floatObjectOf(sqrt(doubleValue));

    /* begin stackTopPut: */
    longAtput(stackPointer, aValue);
  } else {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}