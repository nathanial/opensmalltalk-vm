/* Extracted from interp.c:21910 (function primitiveSecondsClock). */

/*	Return the number of seconds since January 1, 1901 as an integer. */

	/* InterpreterPrimitives>>#primitiveSecondsClock */

static void
primitiveSecondsClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = ((((((usqInt)(ioSecondsNow()))) & 0xFFFFFFFFU) << 3) | 1);

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer),oop);
	stackPointer = sp;
}