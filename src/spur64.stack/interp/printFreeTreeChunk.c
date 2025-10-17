/* Extracted from interp.c:43356 (function printFreeTreeChunk). */

/*	Slang is blind-sided by the inlining of printFreeTreeChunk: into
        printFreeTree.
 */

/* SpurMemoryManager>>#printFreeTreeChunk: */

static NoDbgRegParms void printFreeTreeChunk(sqInt chunkOrZero) {
  if (chunkOrZero > 0) {
    printFreeTreeChunk(longAt(
        (void *)((chunkOrZero + BaseHeaderSize) + (3U << (shiftForWord())))));
    printFreeChunk(chunkOrZero);
    printFreeTreeChunk(longAt(
        (void *)((chunkOrZero + BaseHeaderSize) + (4U << (shiftForWord())))));
  }
}