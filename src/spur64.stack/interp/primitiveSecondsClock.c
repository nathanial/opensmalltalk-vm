/* Extracted from interp.c:21910 (function primitiveSecondsClock). */

static void
primitiveSecondsClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = ((((((usqInt)(ioSecondsNow()))) & 0xFFFFFFFFU) << 3) | 1);

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),oop);
	GIV(stackPointer) = sp;
}