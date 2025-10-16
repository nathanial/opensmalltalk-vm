/* Extracted from interp.c:18992 (function primitiveLocalMicrosecondClock). */

static void
primitiveLocalMicrosecondClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = positive64BitIntegerFor(ioLocalMicrosecondsNow());

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),oop);
	GIV(stackPointer) = sp;
}