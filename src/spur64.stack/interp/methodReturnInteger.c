/* Extracted from interp.c:57818 (function methodReturnInteger). */

/*	Sets the return value for a method. In the CoInterpreter we replace the
	cumbersome primResult machinery. */

	/* StackInterpreter>>#methodReturnInteger: */

sqInt
methodReturnInteger(sqInt integer)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	assert(!((failed())));

	/* begin pop:thenPushInteger: */
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),(((usqInt)integer << 3) | 1));
	stackPointer = sp;
	return 0;
}