/* Extracted from interp.c:25631 (function primitiveUTCMicrosecondClock). */

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