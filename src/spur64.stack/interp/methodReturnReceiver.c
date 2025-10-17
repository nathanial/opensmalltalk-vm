/* Extracted from interp.c:57835 (function methodReturnReceiver). */

/*	Sets the return value for a method */

	/* StackInterpreter>>#methodReturnReceiver */

sqInt
methodReturnReceiver(void)
{
	assert(!((failed())));

	/* begin pop: */
	stackPointer += argumentCount * BytesPerWord;
	return 0;
}