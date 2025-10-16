/* Extracted from interp.c:22971 (function primitiveSmallFloatCosine). */

/* InterpreterPrimitives>>#primitiveSmallFloatCosine */

static void primitiveSmallFloatCosine(void) {
  sqInt aValue;
  sqInt oop;
  double rcvr;

  oop = longAt(stackPointer);

  rcvr = smallFloatValueOf(oop);
  aValue = floatObjectOf(cos(rcvr));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}