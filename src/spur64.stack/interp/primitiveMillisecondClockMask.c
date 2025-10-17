/* Extracted from interp.c:19205 (function primitiveMillisecondClockMask). */

/*	Provide access to the millisecond clock mask to support calculation
	of durations based on the millisecond clock value. */

	/* InterpreterPrimitives>>#primitiveMillisecondClockMask */

EXPORT(sqInt)
primitiveMillisecondClockMask(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),(((usqInt)MillisecondClockMask << 3) | 1));
	GIV(stackPointer) = sp;
	return 0;
}