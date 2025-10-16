/* Extracted from interp.c:21910 (function primitiveSecondsClock). */

/*	Return the number of seconds since January 1, 1901 as an integer. */

/* InterpreterPrimitives>>#primitiveSecondsClock */

static void primitiveSecondsClock(void) {
  sqInt oop;

  oop = ((((((usqInt)(ioSecondsNow()))) & 0xFFFFFFFFU) << 3) | 1);

  popthenPush(1, oop);
}
