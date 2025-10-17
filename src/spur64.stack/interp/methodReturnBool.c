/* Extracted from interp.c:57780 (function methodReturnBool). */

/*	Sets the return value for a method. In the CoInterpreter we replace the
	cumbersome primResult machinery. */

	/* StackInterpreter>>#methodReturnBool: */

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