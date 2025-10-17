/* Extracted from interp.c:23605 (function primitiveSmallFloatLogN). */

/*	Natural log. */

/* InterpreterPrimitives>>#primitiveSmallFloatLogN */

static void primitiveSmallFloatLogN(void) {
  sqInt aValue;
  usqLong bits;
  sqInt oop;
  double rcvr;

  oop = longAt(stackPointer);

  /* begin smallFloatValueOf: */
  /* begin smallFloatBitsOf: */
  assert(isImmediateFloat(oop));
  bits = ((((usqInt)oop))) >> (numTagBits());
  if (bits > 1) {
    bits += (((usqInt)((smallFloatExponentOffset()))
              << ((smallFloatMantissaBits()) + 1)));
  }

  /* a.k.a. ~= +/-0.0 */
  bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
  memcpy((&rcvr), (&bits), sizeof(rcvr));
  aValue = floatObjectOf(log(rcvr));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}