/* Extracted from interp.c:21135 (function primitiveProfilePrimitive). */

/*	Primitive. Answer the last primitive method sampled by the profiler. */

	/* InterpreterPrimitives>>#primitiveProfilePrimitive */

EXPORT(sqInt)
primitiveProfilePrimitive(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = (!(GIV(profileMethod))
			? GIV(nilObj)
			: GIV(profileMethod));

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
	GIV(stackPointer) = sp;
	return 0;
}