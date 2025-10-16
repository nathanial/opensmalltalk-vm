/* Extracted from interp.c:17996 (function primitiveInputWord). */

static void
primitiveInputWord(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer)),(((usqInt)0 << 3) | 1));
	GIV(stackPointer) = sp;
}