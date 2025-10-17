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

  rcvr = smallFloatValueOf(oop);
  frac = modf(rcvr, (&trunc));
  aValue = floatObjectOf(frac);

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}