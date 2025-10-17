/* Extracted from interp.c:25631 (function primitiveUTCMicrosecondClock). */

/*	Return the value of the microsecond clock in UTC as an integer.
	This is the number of microseconds since the Smalltalk epoch, 1901/1/1
	12:00am. The microsecond clock is at least 60 bits wide which means it'll
	get to around August
	38435 before it wraps around. Be sure to put it on your calendar. This
	primitive accesses the time as answered by the OS. */

	/* InterpreterPrimitives>>#primitiveUTCMicrosecondClock */

static void
primitiveUTCMicrosecondClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = positive64BitIntegerFor(ioUTCMicrosecondsNow());

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),oop);
	GIV(stackPointer) = sp;
}