/* Extracted from interp.c:57879 (function methodReturnValue). */

/*	Sets the return value for a method. In the CoInterpreter we replace the
	cumbersome primResult machinery. */

	/* StackInterpreter>>#methodReturnValue: */

sqInt
methodReturnValue(sqInt oop)
{
    char *sp;

	assert(!((failed())));

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),oop);
	stackPointer = sp;
	return 0;
}