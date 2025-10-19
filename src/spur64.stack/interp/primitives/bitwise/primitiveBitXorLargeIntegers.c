/* Extracted from interp.c:13144 (function primitiveBitXorLargeIntegers). */

/*	Primitive logical operations for large integers in 64 bit range */

/* InterpreterPrimitives>>#primitiveBitXorLargeIntegers */

EXPORT(void)
primitiveBitXorLargeIntegers(void) {
  usqLong integerArg;
  usqLong integerRcvr;
  sqInt oopResult;
  char *sp;

  integerArg = positive64BitValueOf(longAt(stackPointer));
  integerRcvr = positive64BitValueOf(longAt(stackPointer + (1 * BytesPerWord)));
  if (primFailCode) {
    return;
  }
  oopResult = positive64BitIntegerFor(integerRcvr ^ integerArg);
  if (!primFailCode) {
    popthenPush(2, oopResult);
  }
}