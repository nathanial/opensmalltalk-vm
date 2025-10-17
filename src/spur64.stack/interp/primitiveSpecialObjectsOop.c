/* Extracted from interp.c:24094 (function primitiveSpecialObjectsOop). */

/*	Return the oop of the SpecialObjectsArray. */

	/* InterpreterPrimitives>>#primitiveSpecialObjectsOop */

static void
primitiveSpecialObjectsOop(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer)),GIV(specialObjectsOop));
	GIV(stackPointer) = sp;
}