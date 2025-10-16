/* Extracted from interp.c:57780 (function methodReturnBool). */

sqInt
methodReturnBool(sqInt boolean)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	assert(!((failed())));

	/* begin pop:thenPushBool: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),/* booleanObjectOf: */
		(boolean
			? GIV(trueObj)
			: GIV(falseObj)));
	GIV(stackPointer) = sp;
	return 0;
}