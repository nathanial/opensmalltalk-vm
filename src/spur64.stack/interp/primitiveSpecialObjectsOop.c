/* Extracted from interp.c:24094 (function primitiveSpecialObjectsOop). */

/*	Return the oop of the SpecialObjectsArray. */

	/* InterpreterPrimitives>>#primitiveSpecialObjectsOop */

static void
primitiveSpecialObjectsOop(void)
{
    char *sp;

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer),specialObjectsOop);
	stackPointer = sp;
}