/* Extracted from interp.c:19205 (function primitiveMillisecondClockMask). */

/*	Provide access to the millisecond clock mask to support calculation
        of durations based on the millisecond clock value. */

/* InterpreterPrimitives>>#primitiveMillisecondClockMask */

EXPORT(sqInt)
primitiveMillisecondClockMask(void) {

  popthenPush(1, (((usqInt)MillisecondClockMask << 3) | 1));
  return 0;
}
