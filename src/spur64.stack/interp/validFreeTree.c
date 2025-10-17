/* Extracted from interp.c:46773 (function validFreeTree). */

/*	useful for debugging */

	/* SpurMemoryManager>>#validFreeTree */

int
validFreeTree(void)
{
	return !(validFreeTreeChunkparent(freeLists[0], 0));
}