/* Extracted from interp.c:14177 (function primitiveCoarseUTCMicrosecondClock). */

/*	Return the value of the microsecond clock as updated by the heartbeat as
	an integer.
	This is the number of microseconds since the Smalltalk epoch, 1901/1/1
	12:00am. The microsecond clock is at least 60 bits wide which means it'll
	get to around August
	38435 before it wraps around. Be sure to put it on your calendar. The
	coarse clock is
	updated by the heartbeat thread and as such is much cheaper than
	primitiveUTCMicrosecondClock, which always entails a system call. */

	/* InterpreterPrimitives>>#primitiveCoarseUTCMicrosecondClock */

static void
primitiveCoarseUTCMicrosecondClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = positive64BitIntegerFor(ioUTCMicroseconds());

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer),oop);
	stackPointer = sp;
}