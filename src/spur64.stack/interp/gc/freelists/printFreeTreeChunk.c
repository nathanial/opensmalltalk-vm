/* Extracted from interp.c:43356 (function printFreeTreeChunk). */

/*	Slang is blind-sided by the inlining of printFreeTreeChunk: into
        printFreeTree.
 */

/* SpurMemoryManager>>#printFreeTreeChunk: */

static void printFreeTreeChunk(sqInt chunkOrZero) {
  if (chunkOrZero > 0) {
    printFreeTreeChunk(fetchPointerofObject(3U, chunkOrZero));
    printFreeChunk(chunkOrZero);
    printFreeTreeChunk(fetchPointerofObject(4U, chunkOrZero));
  }
}