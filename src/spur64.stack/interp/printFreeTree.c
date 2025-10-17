/* Extracted from interp.c:43344 (function printFreeTree). */

/*	useful for VM debugging */

/* SpurMemoryManager>>#printFreeTree */

void printFreeTree(void) { printFreeTreeChunk(freeLists[0]); }