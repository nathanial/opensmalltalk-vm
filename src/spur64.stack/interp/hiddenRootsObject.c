/* Extracted from interp.c:36223 (function hiddenRootsObject). */

	/* SpurMemoryManager>>#hiddenRootsObject */

static sqInt
hiddenRootsObject(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(hiddenRootsObj);
}