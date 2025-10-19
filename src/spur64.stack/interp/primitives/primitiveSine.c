/* Extracted from interp.c:22609 (function primitiveSine). */

/*	Computes sine of float receiver; receiver *must* be a float instance. */

/* InterpreterPrimitives>>#primitiveSine */

static void primitiveSine(void) {
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
  aValue = floatObjectOf(sin(doubleValue));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}