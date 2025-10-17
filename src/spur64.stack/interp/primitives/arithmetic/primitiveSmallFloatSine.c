/* Extracted from interp.c:23798 (function primitiveSmallFloatSine). */

/* InterpreterPrimitives>>#primitiveSmallFloatSine */

static void primitiveSmallFloatSine(void) {
  sqInt aValue;
  usqLong bits;
  sqInt oop;
  double rcvr;

  oop = longAt(stackPointer);

  rcvr = smallFloatValueOf(oop);
  aValue = floatObjectOf(sin(rcvr));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}