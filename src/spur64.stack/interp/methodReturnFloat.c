/* Extracted from interp.c:57800 (function methodReturnFloat). */

sqInt
methodReturnFloat(double aFloat)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	assert(!((failed())));

	/* begin pop:thenPushFloat: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),floatObjectOf(aFloat));
	GIV(stackPointer) = sp;
	return 0;
}