/* Extracted from interp.c:43344 (function printFreeTree). */

/*	useful for VM debugging */

	/* SpurMemoryManager>>#printFreeTree */

void
printFreeTree(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	printFreeTreeChunk(GIV(freeLists)[0]);
}