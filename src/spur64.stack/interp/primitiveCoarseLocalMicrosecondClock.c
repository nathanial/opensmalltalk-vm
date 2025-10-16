/* Extracted from interp.c:14152 (function primitiveCoarseLocalMicrosecondClock). */

static void
primitiveCoarseLocalMicrosecondClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = positive64BitIntegerFor(ioLocalMicroseconds());

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),oop);
	GIV(stackPointer) = sp;
}