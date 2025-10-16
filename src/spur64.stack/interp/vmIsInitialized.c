/* Extracted from interp.c:11036 (function vmIsInitialized). */

int
vmIsInitialized(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return !!GIV(pages);
}