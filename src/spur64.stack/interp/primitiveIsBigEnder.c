/* Extracted from interp.c:18680 (function primitiveIsBigEnder). */

/*	Answer if running on a big endian machine. */

	/* InterpreterPrimitives>>#primitiveIsBigEnder */

EXPORT(sqInt)
primitiveIsBigEnder(void)
{
    char *sp;

	/* begin pop:thenPushBool: */
	longAtput((sp = stackPointer),falseObj);
	stackPointer = sp;
	return 0;
}