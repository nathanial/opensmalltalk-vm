/* Extracted from interp.c:57818 (function methodReturnInteger). */

sqInt
methodReturnInteger(sqInt integer)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	assert(!((failed())));

	/* begin pop:thenPushInteger: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),(((usqInt)integer << 3) | 1));
	GIV(stackPointer) = sp;
	return 0;
}