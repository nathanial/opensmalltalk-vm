/* Extracted from interp.c:21159 (function primitiveProfileSample). */

/*	Primitive. Answer the last sample taken by the profiler, or nil if the
	profiler isn't active.
	See also primitiveProfileStart.
 */

	/* InterpreterPrimitives>>#primitiveProfileSample */

EXPORT(sqInt)
primitiveProfileSample(void)
{
    sqInt oop;
    char *sp;

	oop = (!(profileProcess)
			? nilObj
			: profileProcess);

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),oop);
	stackPointer = sp;
	return 0;
}