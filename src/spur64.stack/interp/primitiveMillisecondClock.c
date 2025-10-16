/* Extracted from interp.c:19187 (function primitiveMillisecondClock). */

static void
primitiveMillisecondClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = ((((ioMSecs()) & MillisecondClockMask) << 3) | 1);

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),oop);
	GIV(stackPointer) = sp;
}