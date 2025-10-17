/* Extracted from interp.c:16616 (function primitiveFractionalPart). */

/*	Fractional part of float receiver; receiver *must* be a float instance.
 */

/* InterpreterPrimitives>>#primitiveFractionalPart */

static void primitiveFractionalPart(void) {
  sqInt aValue;
  usqLong bits;
  double doubleValue;
  sqInt rcvr;
  double trunc;

  rcvr = longAt(stackPointer);

  /* begin noFailFloatValueOf: */
  assert(isFloatInstance(rcvr));
  if (rcvr & (tagMask())) {
    doubleValue = smallFloatValueOf(rcvr);
  } else {
    fetchFloatAtinto(rcvr + BaseHeaderSize, doubleValue);
  }
  aValue = floatObjectOf(modf(doubleValue, (&trunc)));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}