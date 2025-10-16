/* Extracted from interp.c:26399 (function pageIndexFor). */

static NoDbgRegParms sqInt
pageIndexFor(void *pointer)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return pageIndexForstackMemorybytesPerPage(pointer, GIV(stackMemory), GIV(bytesPerPage));
}