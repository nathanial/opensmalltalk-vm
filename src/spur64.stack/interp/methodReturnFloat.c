/* Extracted from interp.c:57800 (function methodReturnFloat). */

/*	Sets the return value for a method. */

	/* StackInterpreter>>#methodReturnFloat: */

sqInt
methodReturnFloat(double aFloat)
{
    char *sp;

	assert(!((failed())));

	/* begin pop:thenPushFloat: */
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),floatObjectOf(aFloat));
	stackPointer = sp;
	return 0;
}