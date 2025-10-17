/* Extracted from interp.c:25313 (function primitiveTimesTwoPower). */

/* TestingPrimitives */

/*	Multiply the receiver by the power of the argument.
        Receiver *must* be a float instance. */

/* InterpreterPrimitives>>#primitiveTimesTwoPower */

static void primitiveTimesTwoPower(void) {
  sqInt aFloatOop;
  sqInt arg;
  usqLong bits;
  double rcvr;
  double result;
  char *sp;
  sqInt twiceMaxExponent;

  arg = longAt(stackPointer);
  if (!((((arg) & 7) == 1))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadArgument;
    return;
  }
  arg = (arg >> 3);
  twiceMaxExponent = 0x1000;
  if (arg < (-twiceMaxExponent)) {
    arg = -twiceMaxExponent;
  } else {
    if (arg > twiceMaxExponent) {
      arg = twiceMaxExponent;
    }
  }

  /* clip arg to at most int range; ldexp's last arg is of type int */
  aFloatOop = longAt(stackPointer + (1 * BytesPerWord));

  /* begin noFailFloatValueOf: */
  assert(isFloatInstance(aFloatOop));
  if (aFloatOop & (tagMask())) {
    /* begin smallFloatValueOf: */
    /* begin smallFloatBitsOf: */
    assert(isImmediateFloat(aFloatOop));
    bits = ((((usqInt)aFloatOop))) >> (numTagBits());
    if (bits > 1) {
      bits += (((usqInt)((smallFloatExponentOffset()))
                << ((smallFloatMantissaBits()) + 1)));
    }

    /* a.k.a. ~= +/-0.0 */
    bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
    memcpy((&rcvr), (&bits), sizeof(rcvr));
  } else {
    fetchFloatAtinto(aFloatOop + BaseHeaderSize, rcvr);
  }
  result = ldexp(rcvr, ((int)arg));

  /* begin pop:thenPushFloat: */
  longAtput((sp = stackPointer + (1 * BytesPerWord)), floatObjectOf(result));
  stackPointer = sp;
}