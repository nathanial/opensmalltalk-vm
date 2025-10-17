/* Extracted from interp.c:11013 (function stackPageFor). */

/*	<Integer> */

	/* CogStackPages>>#stackPageFor: */

static NoDbgRegParms StackPage *
stackPageFor(void *pointer)
{
	return stackPageAtpages(pageIndexForstackMemorybytesPerPage(pointer, stackMemory, bytesPerPage), pages);
}