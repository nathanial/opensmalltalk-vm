/* Extracted from interp.c:11013 (function stackPageFor). */

/*	<Integer> */

	/* CogStackPages>>#stackPageFor: */

static NoDbgRegParms StackPage *
stackPageFor(void *pointer)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return stackPageAtpages(pageIndexForstackMemorybytesPerPage(pointer, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
}