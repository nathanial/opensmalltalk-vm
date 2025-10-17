/* Extracted from interp.c:46645 (function updateListStartingAt). */

	/* SpurMemoryManager>>#updateListStartingAt: */

static NoDbgRegParms void
updateListStartingAt(sqInt freeNode)
{
    sqInt obj;
    sqInt prev;

	obj = 0;
	if (!freeNode) {
		return;
	}
	assert(!((isLilliputianSize(bytesInBody(freeNode)))));
	prev = freeNode;

	/* begin storePointer:ofFreeChunk:withValue: */
	assert(isFreeObject(prev));
	longAtput((void *)((prev + BaseHeaderSize) + (1U << (shiftForWord()))),0);
	while (1) {
		obj = longAt((void *)((prev + BaseHeaderSize) + (0U << (shiftForWord()))));
		if (!(obj != 0)) break;
		/* begin storePointer:ofFreeChunk:withValue: */
		assert(isFreeObject(obj));
		assert((prev == 0)
		 || (isFreeObject(prev)));
		longAtput((void *)((obj + BaseHeaderSize) + (1U << (shiftForWord()))),prev);
		prev = obj;
	}
}