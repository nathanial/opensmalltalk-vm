/* Extracted from interp.c:14177 (function primitiveCoarseUTCMicrosecondClock). */

static void
primitiveCoarseUTCMicrosecondClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = positive64BitIntegerFor(ioUTCMicroseconds());

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),oop);
	GIV(stackPointer) = sp;
}