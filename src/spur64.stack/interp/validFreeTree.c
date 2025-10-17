/* Extracted from interp.c:46773 (function validFreeTree). */

/*	useful for debugging */

	/* SpurMemoryManager>>#validFreeTree */

int
validFreeTree(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return !(validFreeTreeChunkparent(GIV(freeLists)[0], 0));
}