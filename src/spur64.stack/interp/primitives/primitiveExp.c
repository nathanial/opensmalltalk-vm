/* Extracted from interp.c:15147 (function primitiveExp). */

/*	Computes E raised to the receiver power.
        Receiver *must* be a float instance. */

/* InterpreterPrimitives>>#primitiveExp */

static void primitiveExp(void) {
  sqInt aValue;
  usqLong bits;
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
  aValue = floatObjectOf(exp(doubleValue));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}