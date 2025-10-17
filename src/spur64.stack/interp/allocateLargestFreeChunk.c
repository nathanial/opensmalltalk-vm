/* Extracted from interp.c:30715 (function allocateLargestFreeChunk). */

/*	Answer the largest free chunk in the free lists. */
/*	would like to use ifNotNil: but the ^next inside the ^blah ifNotNil:
	confused Slang
 */

	/* SpurMemoryManager>>#allocateLargestFreeChunk */

static sqInt
allocateLargestFreeChunk(void)
{
    sqInt freeChunk;
    sqInt next;
    sqInt nextFreeChunk;

	freeChunk = findLargestFreeChunk();
	if (!freeChunk) {
		return null;
	}

	/* This will be the node, not a list element.  Answer a list element in preference. */
	next = longAt((void *)((freeChunk + BaseHeaderSize) + (0U << (shiftForWord()))));
	if (next) {
		assert((bytesInBody(freeChunk)) >= (numFreeLists()));
		nextFreeChunk = longAt((void *)((next + BaseHeaderSize) + (0U << (shiftForWord()))));

		/* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
		/* begin storePointer:ofFreeChunk:withValue: */
		assert(isFreeObject(freeChunk));
		assert((nextFreeChunk == 0)
		 || (isFreeObject(nextFreeChunk)));
		longAtput((void *)((freeChunk + BaseHeaderSize) + (0U << (shiftForWord()))),nextFreeChunk);
		if (nextFreeChunk) {
			/* begin storePointer:ofFreeChunk:withValue: */
			assert(isFreeObject(nextFreeChunk));
			assert((freeChunk == 0)
			 || (isFreeObject(freeChunk)));
			longAtput((void *)((nextFreeChunk + BaseHeaderSize) + (1U << (shiftForWord()))),freeChunk);
		}
		return next;
	}
	unlinkSolitaryFreeTreeNode(freeChunk);
	return freeChunk;
}