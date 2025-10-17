/* Extracted from interp.c:21135 (function primitiveProfilePrimitive). */

/*	Primitive. Answer the last primitive method sampled by the profiler. */

	/* InterpreterPrimitives>>#primitiveProfilePrimitive */

EXPORT(sqInt)
primitiveProfilePrimitive(void)
{
    sqInt oop;
    char *sp;

	oop = (!(profileMethod)
			? nilObj
			: profileMethod);

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),oop);
	stackPointer = sp;
	return 0;
}