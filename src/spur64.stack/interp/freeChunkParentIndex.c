/* Extracted from interp.c:35797 (function freeChunkParentIndex). */

/*	for organizing the tree of large free chunks. */

	/* SpurMemoryManager>>#freeChunkParentIndex */

static sqInt
freeChunkParentIndex(void)
{
	return 2;
}