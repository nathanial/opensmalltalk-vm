/* Extracted from interp.c:57800 (function methodReturnFloat). */

/*	Sets the return value for a method. */

	/* StackInterpreter>>#methodReturnFloat: */

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