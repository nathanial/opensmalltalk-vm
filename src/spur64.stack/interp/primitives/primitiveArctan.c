/* Extracted from interp.c:11819 (function primitiveArctan). */

/*	Computes arctan of float receiver; receiver *must* be a float instance.
 */

/* InterpreterPrimitives>>#primitiveArctan */

static void primitiveArctan(void) {
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
  aValue = floatObjectOf(atan(doubleValue));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}