/* Extracted from interp.c:12050 (function primitiveAsFloat). */

/*	N.B. This will answer inexact results for integers with > 53 bits of
	magnitude. 
 */

	/* InterpreterPrimitives>>#primitiveAsFloat */

static void
primitiveAsFloat(void)
{
    sqInt rcvr;
    char *sp;

	rcvr = longAt(stackPointer);
	assert((((rcvr) & 7) == 1));

	/* begin pop:thenPushFloat: */
	longAtput((sp = stackPointer),floatObjectOf(((double) ((rcvr >> 3)) )));
	stackPointer = sp;
}