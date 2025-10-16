/* Extracted from interp.c:43312 (function printFreeList). */

/*	useful for VM debugging */

/* SpurMemoryManager>>#printFreeList: */

sqInt printFreeList(sqInt chunkOrIndex) {
  sqInt freeChunk;

  if ((chunkOrIndex >= 0) && (chunkOrIndex < 64 /* numFreeLists */)) {
    return printFreeList(freeLists[chunkOrIndex]);
  }
  freeChunk = chunkOrIndex;
  while (freeChunk != 0) {
    printFreeChunk(freeChunk);
    freeChunk = fetchPointerofObject(0U, freeChunk);
  }
  return 0;
}