/* Extracted from interp.c:12050 (function primitiveAsFloat). */

/*	N.B. This will answer inexact results for integers with > 53 bits of
	magnitude. 
 */

	/* InterpreterPrimitives>>#primitiveAsFloat */

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