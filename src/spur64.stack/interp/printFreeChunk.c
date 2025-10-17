/* Extracted from interp.c:43193 (function printFreeChunk). */

/*	useful for VM debugging */

	/* SpurMemoryManager>>#printFreeChunk: */

void
printFreeChunk(sqInt freeChunk)
{
	printFreeChunkprintAsTreeNode(freeChunk, 1);
}