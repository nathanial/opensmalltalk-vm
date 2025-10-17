/* Extracted from interp.c:22943 (function primitiveSmallFloatArctan). */

/* InterpreterPrimitives>>#primitiveSmallFloatArctan */

static void primitiveSmallFloatArctan(void) {
  sqInt aValue;
  usqLong bits;
  sqInt oop;
  double rcvr;

  oop = longAt(stackPointer);

  rcvr = smallFloatValueOf(oop);
  aValue = floatObjectOf(atan(rcvr));

  /* begin stackTopPut: */
  longAtput(stackPointer, aValue);
}