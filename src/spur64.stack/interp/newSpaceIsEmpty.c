/* Extracted from interp.c:40759 (function newSpaceIsEmpty). */

	/* SpurMemoryManager>>#newSpaceIsEmpty */

static sqInt
newSpaceIsEmpty(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (freeStart == (((eden).start)))
	 && (pastSpaceStart == (((pastSpace).start)));
}