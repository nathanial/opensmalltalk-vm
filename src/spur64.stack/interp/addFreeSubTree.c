/* Extracted from interp.c:29651 (function addFreeSubTree). */

static NoDbgRegParms void
addFreeSubTree(sqInt freeTree)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt bytesInArg;
    usqInt bytesInNode;
    sqInt subNode;
    sqInt treeNode;

	assert(isFreeObject(freeTree));
	bytesInArg = bytesInBody(freeTree);
	assert(bytesInArg >= ((numFreeLists()) * (allocationUnit())));
	treeNode = GIV(freeLists)[0];
	assert(treeNode != 0);
	while (1) {
		bytesInNode = bytesInBody(treeNode);

		/* check for overlap; could write this as self oop: (self objectAfter: freeChunk) isLessThanOrEqualTo: child...
		   but that relies on headers being correct, etc.  So keep it clumsy... */
		assert((oopisLessThanOrEqualTo((freeTree + bytesInArg) - BaseHeaderSize, treeNode))
		 || (oopisGreaterThanOrEqualTo(freeTree, (treeNode + bytesInNode) - BaseHeaderSize)));
		assert(bytesInNode >= ((numFreeLists()) * (allocationUnit())));
		assert(bytesInArg != bytesInNode);
		if (bytesInNode > bytesInArg) {
			subNode = longAt((void *)((treeNode + BaseHeaderSize) + (3U << (shiftForWord()))));
			if (!subNode) {
				/* begin storePointer:ofFreeChunk:withValue: */
				assert(isFreeObject(treeNode));
				assert((freeTree == 0)
				 || (isFreeObject(freeTree)));
				longAtput((void *)((treeNode + BaseHeaderSize) + (3U << (shiftForWord()))),freeTree);

				/* begin storePointer:ofFreeChunk:withValue: */
				assert(isFreeObject(freeTree));
				assert((treeNode == 0)
				 || (isFreeObject(treeNode)));
				longAtput((void *)((freeTree + BaseHeaderSize) + (2U << (shiftForWord()))),treeNode);
				return;
			}
		}
		else {
			subNode = longAt((void *)((treeNode + BaseHeaderSize) + (4U << (shiftForWord()))));
			if (!subNode) {
				/* begin storePointer:ofFreeChunk:withValue: */
				assert(isFreeObject(treeNode));
				assert((freeTree == 0)
				 || (isFreeObject(freeTree)));
				longAtput((void *)((treeNode + BaseHeaderSize) + (4U << (shiftForWord()))),freeTree);

				/* begin storePointer:ofFreeChunk:withValue: */
				assert(isFreeObject(freeTree));
				assert((treeNode == 0)
				 || (isFreeObject(treeNode)));
				longAtput((void *)((freeTree + BaseHeaderSize) + (2U << (shiftForWord()))),treeNode);
				return;
			}
		}
		treeNode = subNode;
	}
}