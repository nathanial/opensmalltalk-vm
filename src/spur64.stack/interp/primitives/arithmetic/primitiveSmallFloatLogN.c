/* Extracted from interp.c:23605 (function primitiveSmallFloatLogN). */

/*	Natural log. */

/* InterpreterPrimitives>>#primitiveSmallFloatLogN */

static void primitiveSmallFloatLogN(void) {
  sqInt aValue;
  usqLong bits;
  sqInt oop;
  double rcvr;

  oop = longAt(stackPointer);

  rcvr = smallFloatValueOf(oop);
  aValue = floatObjectOf(log(rcvr));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}