/* Extracted from interp.c:36856 (function inOrderPrintFreeTreeprintList). */

/*	print free chunks in freeTree in order. */
/*	useful for VM debugging */

/* SpurMemoryManager>>#inOrderPrintFreeTree:printList: */

void inOrderPrintFreeTreeprintList(sqInt freeChunk, sqInt printNextList) {
  sqInt next;

  if ((next = fetchPointerofObject(3U, freeChunk))) {
    inOrderPrintFreeTreeprintList(next, printNextList);
  }
  printFreeChunkprintAsTreeNode(freeChunk, 1);
  if (printNextList) {
    next = freeChunk;
    while (((next = fetchPointerofObject(0U, next))) != 0) {
      /* begin tab */
      printChar('\t');
      printFreeChunkprintAsTreeNode(next, 0);
    }
  }
  if ((next = fetchPointerofObject(4U, freeChunk))) {
    inOrderPrintFreeTreeprintList(next, printNextList);
  }
}