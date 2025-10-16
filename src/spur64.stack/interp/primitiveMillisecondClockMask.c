/* Extracted from interp.c:19205 (function primitiveMillisecondClockMask). */

EXPORT(sqInt)
primitiveMillisecondClockMask(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),(((usqInt)MillisecondClockMask << 3) | 1));
	GIV(stackPointer) = sp;
	return 0;
}