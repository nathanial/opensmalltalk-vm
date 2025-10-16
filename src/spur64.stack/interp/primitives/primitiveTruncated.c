/* Extracted from interp.c:25373 (function primitiveTruncated). */

/*	Integral part of float receiver; receiver *must* be a float instance. */

/* InterpreterPrimitives>>#primitiveTruncated */

static void primitiveTruncated(void) {
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
  modf(doubleValue, (&trunc));
  if (((trunc >= (((double)(MinSmallInteger)))) &&
       (trunc <= (((double)(MaxSmallInteger)))))) {
    /* stackTopPut: */
    longAtput(stackPointer, (((usqInt)(((sqInt)trunc)) << 3) | 1));
    return;
  }

  /* make Nicolas Cellier's otherwise recursive BoxedFloat64>>truncated nice
   * 2/7/2025 07:34 work on 32-bits. */

  /* a.k.a. trunc abs <= Float maxExactInteger asFloat */

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
}
