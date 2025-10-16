/* Extracted from interp.c:12050 (function primitiveAsFloat). */

static void
primitiveAsFloat(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt rcvr;
    char *sp;

	rcvr = longAt(GIV(stackPointer));
	assert((((rcvr) & 7) == 1));

	/* begin pop:thenPushFloat: */
	longAtput((sp = GIV(stackPointer)),floatObjectOf(((double) ((rcvr >> 3)) )));
	GIV(stackPointer) = sp;
}