/* Extracted from interp.c:19009 (function primitiveLogN). */

/*	Natural log of float receiver; receiver *must* be a float instance. */

/* InterpreterPrimitives>>#primitiveLogN */

static void primitiveLogN(void) {
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
  aValue = floatObjectOf(log(doubleValue));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}