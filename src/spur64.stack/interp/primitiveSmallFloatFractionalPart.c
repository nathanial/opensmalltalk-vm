/* Extracted from interp.c:23207 (function primitiveSmallFloatFractionalPart).
 */

/* InterpreterPrimitives>>#primitiveSmallFloatFractionalPart */

static void primitiveSmallFloatFractionalPart(void) {
  sqInt aValue;
  usqLong bits;
  double frac;
  sqInt oop;
  double rcvr;
  double trunc;

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
  frac = modf(rcvr, (&trunc));
  aValue = floatObjectOf(frac);

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}