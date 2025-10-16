/* Extracted from interp.c:57835 (function methodReturnReceiver). */

sqInt
methodReturnReceiver(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	assert(!((failed())));

	/* begin pop: */
	GIV(stackPointer) += GIV(argumentCount) * BytesPerWord;
	return 0;
}