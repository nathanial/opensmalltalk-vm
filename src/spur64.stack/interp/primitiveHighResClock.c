/* Extracted from interp.c:17714 (function primitiveHighResClock). */

EXPORT(sqInt)
primitiveHighResClock(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt oop;
    char *sp;

	oop = positive64BitIntegerFor(ioHighResClock());

	/* begin methodReturnValue: */
	assert(!((failed())));
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),oop);
	GIV(stackPointer) = sp;
	return 0;
}