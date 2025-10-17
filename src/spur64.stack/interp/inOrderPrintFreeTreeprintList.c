/* Extracted from interp.c:36856 (function inOrderPrintFreeTreeprintList). */

/*	print free chunks in freeTree in order. */
/*	useful for VM debugging */

	/* SpurMemoryManager>>#inOrderPrintFreeTree:printList: */

void
inOrderPrintFreeTreeprintList(sqInt freeChunk, sqInt printNextList)
{
    sqInt next;

	if ((next = longAt((void *)((freeChunk + BaseHeaderSize) + (3U << (shiftForWord())))))) {
		inOrderPrintFreeTreeprintList(next, printNextList);
	}
	printFreeChunkprintAsTreeNode(freeChunk, 1);
	if (printNextList) {
		next = freeChunk;
		while (((next = longAt((void *)((next + BaseHeaderSize) + (0U << (shiftForWord())))))) != 0) {
			/* begin tab */
			printChar('\t');
			printFreeChunkprintAsTreeNode(next, 0);
		}
	}
	if ((next = longAt((void *)((freeChunk + BaseHeaderSize) + (4U << (shiftForWord())))))) {
		inOrderPrintFreeTreeprintList(next, printNextList);
	}
}