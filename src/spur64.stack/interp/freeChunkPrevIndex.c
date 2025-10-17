/* Extracted from interp.c:35808 (function freeChunkPrevIndex). */

/*	For linking objecs on each free list, doubly-linking the free objects.
	Free chunks of size 1 do not have a prev index. */

	/* SpurMemoryManager>>#freeChunkPrevIndex */

static sqInt
freeChunkPrevIndex(void)
{
	return 1;
}