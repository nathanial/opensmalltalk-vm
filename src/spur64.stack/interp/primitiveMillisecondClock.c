/* Extracted from interp.c:19187 (function primitiveMillisecondClock). */

/*	Return the value of the millisecond clock as an integer. Note that the
	millisecond clock wraps around periodically. On some platforms it can wrap
	daily. The range is limited to SmallInteger maxVal / 2 to allow delays of
	up to that length without overflowing a SmallInteger.
 */

	/* InterpreterPrimitives>>#primitiveMillisecondClock */

static void
primitiveMillisecondClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = ((((ioMSecs()) & MillisecondClockMask) << 3) | 1);

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer),oop);
	stackPointer = sp;
}