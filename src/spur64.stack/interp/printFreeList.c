/* Extracted from interp.c:43312 (function printFreeList). */

/*	useful for VM debugging */

	/* SpurMemoryManager>>#printFreeList: */

sqInt
printFreeList(sqInt chunkOrIndex)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt freeChunk;

	if ((chunkOrIndex >= 0)
	 && (chunkOrIndex < 64 /* numFreeLists */)) {
		return printFreeList(freeLists[chunkOrIndex]);
	}
	freeChunk = chunkOrIndex;
	while (freeChunk != 0) {
		printFreeChunk(freeChunk);
		freeChunk = longAt((void *)((freeChunk + BaseHeaderSize) + (0U << (shiftForWord()))));
	}
	return 0;
}