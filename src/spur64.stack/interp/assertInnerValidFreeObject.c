/* Extracted from interp.c:31120 (function assertInnerValidFreeObject). */

/*	we don't want to inline so we can nest that in an assertion with the
	return true so the production VM does not generate any code here, while in
	simulation, the code breaks on the assertion we want to.
 */

	/* SpurMemoryManager>>#assertInnerValidFreeObject: */

static NoDbgRegParms NeverInline sqInt
assertInnerValidFreeObject(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt cameFrom;
    sqInt chunk;
    sqInt chunkBytes;
    usqInt index;
    sqInt largeChild;
    sqInt smallChild;
    sqInt treeNode;

	assert(oopisLessThanOrEqualTo(addressAfter(objOop), endOfMemory));
	chunk = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
	assert((chunk == 0)
	 || (isFreeOop(chunk)));
	chunkBytes = bytesInBody(objOop);

	/* begin isLilliputianSize: */
	assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
	if (!(chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */))) {
		chunk = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
		if (chunk) {
			assert(isFreeOop(chunk));
			assert(objOop == (fetchPointerofFreeChunk(freeChunkPrevIndex(), chunk)));
		}
		chunk = longAt((void *)((objOop + BaseHeaderSize) + (1U << (shiftForWord()))));
		index = (bytesInBody(objOop)) / 8 /* allocationUnit */;
		if ((index < 64 /* numFreeLists */)
		 && ((1ULL << index) <= freeListsMask)) {
			if ((freeLists[index]) == objOop) {
				assert(chunk == 0);
			}
		}
		else {
			/* begin freeTreeNodesDo: */
			treeNode = freeLists[0];
			if (!treeNode) {
				goto l1;
			}
			cameFrom = -1;
			do {
				assert((bytesInBody(treeNode)) >= ((numFreeLists()) * (allocationUnit())));
				smallChild = longAt((void *)((treeNode + BaseHeaderSize) + (3U << (shiftForWord()))));
				largeChild = longAt((void *)((treeNode + BaseHeaderSize) + (4U << (shiftForWord()))));
				assert((smallChild == 0)
				 || (treeNode == (fetchPointerofFreeChunk(freeChunkParentIndex(), smallChild))));
				assert((largeChild == 0)
				 || (treeNode == (fetchPointerofFreeChunk(freeChunkParentIndex(), largeChild))));

				/* apply if the node has no children, or it has no large children and we're
				   returning from the small child, or we're returning from the large child. */
				if (((smallChild == 0)
				 && (largeChild == 0))
				 || ((largeChild
						? cameFrom == largeChild
						: cameFrom == smallChild))) {
					if (treeNode == objOop) {
						assert(chunk == 0);
					}

					/* and since we've applied we must move on up */
					cameFrom = treeNode;
					treeNode = longAt((void *)((treeNode + BaseHeaderSize) + (2U << (shiftForWord()))));
				}
				else {
					if ((smallChild != 0)
					 && (cameFrom != smallChild)) {
						treeNode = smallChild;
					}
					else {
						assert(largeChild != 0);
						treeNode = largeChild;
					}
					cameFrom = -1;
				}
			} while(treeNode != 0);
			/* end freeTreeNodesDo: */
l1:;
		}
		if (chunk) {
			assert(isFreeOop(chunk));
			assert(objOop == (fetchPointerofFreeChunk(freeChunkNextIndex(), chunk)));
		}
	}

	/* double linkedlist assertions */
	if ((bytesInBody(objOop)) >= 0x200 /* (numFreeLists * #allocationUnit) */) {
		chunk = longAt((void *)((objOop + BaseHeaderSize) + (2U << (shiftForWord()))));
		assert((chunk == 0)
		 || ((isFreeOop(chunk))
		 && (isLargeFreeObject(chunk))));
		chunk = longAt((void *)((objOop + BaseHeaderSize) + (3U << (shiftForWord()))));
		assert((chunk == 0)
		 || ((isFreeOop(chunk))
		 && (isLargeFreeObject(chunk))));
		chunk = longAt((void *)((objOop + BaseHeaderSize) + (4U << (shiftForWord()))));
		assert((chunk == 0)
		 || ((isFreeOop(chunk))
		 && (isLargeFreeObject(chunk))));
	}

	/* Tree assertions */
	return 1;
}