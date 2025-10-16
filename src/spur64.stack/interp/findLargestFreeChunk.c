/* Extracted from interp.c:35114 (function findLargestFreeChunk). */

static sqInt
findLargestFreeChunk(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt childNode;
    sqInt treeNode;

	childNode = 0;
	treeNode = GIV(freeLists)[0];
	if (!treeNode) {
		return null;
	}
	while (1) {
		/* begin assertValidFreeObject: */
		assert(assertInnerValidFreeObject(treeNode));
		assert((bytesInBody(treeNode)) >= ((numFreeLists()) * (allocationUnit())));
		childNode = longAt((void *)((treeNode + BaseHeaderSize) + (4U << (shiftForWord()))));
		if (!(childNode != 0)) break;
		treeNode = childNode;
	}
	return treeNode;
}