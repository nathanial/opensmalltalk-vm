/* Extracted from interp.c:21816 (function primitiveScreenScaleFactor). */

EXPORT(sqInt)
primitiveScreenScaleFactor(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    double factor;
    char *sp;

	factor = ioScreenScaleFactor();
	if (!GIV(primFailCode)) {
		/* begin methodReturnFloat: */
		assert(!((failed())));
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),floatObjectOf(factor));
		GIV(stackPointer) = sp;
	}
	return 0;
}