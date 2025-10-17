/* Extracted from interp.c:14655 (function primitiveCosine). */

/*	Computes cosine of float receiver; receiver *must* be a float instance.
 */

/* InterpreterPrimitives>>#primitiveCosine */

static void primitiveCosine(void) {
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
  aValue = floatObjectOf(cos(doubleValue));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}