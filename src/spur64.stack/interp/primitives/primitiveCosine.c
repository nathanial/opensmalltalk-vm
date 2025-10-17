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
    /* begin smallFloatValueOf: */
    /* begin smallFloatBitsOf: */
    assert(isImmediateFloat(rcvr));
    bits = ((((usqInt)rcvr))) >> (numTagBits());
    if (bits > 1) {
      bits += (((usqInt)((smallFloatExponentOffset()))
                << ((smallFloatMantissaBits()) + 1)));
    }

    /* a.k.a. ~= +/-0.0 */
    bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
    memcpy((&doubleValue), (&bits), sizeof(doubleValue));
  } else {
    fetchFloatAtinto(rcvr + BaseHeaderSize, doubleValue);
  }
  aValue = floatObjectOf(cos(doubleValue));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}