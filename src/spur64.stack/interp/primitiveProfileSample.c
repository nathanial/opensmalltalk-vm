/* Extracted from interp.c:21159 (function primitiveProfileSample). */

EXPORT(sqInt)
primitiveProfileSample(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = (!(GIV(profileProcess))
			? GIV(nilObj)
			: GIV(profileProcess));

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
	GIV(stackPointer) = sp;
	return 0;
}