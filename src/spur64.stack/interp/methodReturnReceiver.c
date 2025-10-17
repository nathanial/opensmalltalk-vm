/* Extracted from interp.c:57835 (function methodReturnReceiver). */

/*	Sets the return value for a method */

	/* StackInterpreter>>#methodReturnReceiver */

sqInt
methodReturnReceiver(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(!((failed())));

	/* begin pop: */
	stackPointer += argumentCount * BytesPerWord;
	return 0;
}