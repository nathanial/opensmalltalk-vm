/* Extracted from interp.c:30761 (function allocateOldSpaceChunkOfBytes). */

/*	Answer a chunk of oldSpace from the free lists, if available,
	otherwise answer nil. Break up a larger chunk if one of the
	exact size does not exist. N.B. the chunk is simply a pointer, it
	has no valid header. The caller *must* fill in the header correctly. */
/*	for debugging: */
/*	totalFreeOldSpace := self totalFreeListBytes */

	/* SpurMemoryManager>>#allocateOldSpaceChunkOfBytes: */

static NoDbgRegParms sqInt
allocateOldSpaceChunkOfBytes(usqInt chunkBytes)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt child;
    usqInt childBytes;
    sqInt chunk;
    usqInt index;
    usqInt initialIndex;
    sqInt lilliputian;
    sqInt next;
    sqInt nextFreeChunk;
    usqInt nodeBytes;
    sqInt parent;

	nodeBytes = 0;

	/* be optimistic (& don't wait for the write) */
	GIV(totalFreeOldSpace) -= chunkBytes;
	initialIndex = chunkBytes / 8 /* allocationUnit */;
	if ((initialIndex < 64 /* numFreeLists */)
	 && ((1ULL << initialIndex) <= GIV(freeListsMask))) {
		if (((GIV(freeListsMask) & (1ULL << initialIndex)) != 0)) {
			if ((chunk = GIV(freeLists)[initialIndex])) {
				assert(chunk == (startOfObject(chunk)));

				/* begin assertValidFreeObject: */
				assert(assertInnerValidFreeObject(chunk));
				/* begin isLilliputianSize: */
				assert(chunkBytes >= (BaseHeaderSize + (allocationUnit())));
				lilliputian = chunkBytes == (BaseHeaderSize + 8 /* allocationUnit */);

				/* begin unlinkFreeChunk:atIndex:isLilliputianSize: */
				assert(((bytesInBody(chunk)) == (initialIndex * (allocationUnit())))
				 && ((initialIndex > 1)
				 && ((startOfObject(chunk)) == chunk)));

				/* For some reason the assertion is not compiled correctly */
				GIV(freeLists)[initialIndex] = ((next = longAt((void *)((chunk + BaseHeaderSize) + (0U << (shiftForWord()))))));
				if ((!lilliputian)
				 && (next != 0)) {
					/* begin storePointer:ofFreeChunk:withValue: */
					assert(isFreeObject(next));
					longAtput((void *)((next + BaseHeaderSize) + (1U << (shiftForWord()))),0);
				}
				return chunk;
			}
			GIV(freeListsMask) -= 1ULL << initialIndex;
		}

		/* first search for free chunks of a multiple of chunkBytes in size */
		index = initialIndex;
		while ((((index += index)) < 64 /* numFreeLists */)
		 && ((1ULL << index) <= GIV(freeListsMask))) {
			if (((GIV(freeListsMask) & (1ULL << index)) != 0)) {
				if ((chunk = GIV(freeLists)[index])) {
					assert(chunk == (startOfObject(chunk)));

					/* begin assertValidFreeObject: */
					assert(assertInnerValidFreeObject(chunk));
					assert(((bytesInBody(chunk)) == (index * (allocationUnit())))
					 && ((index > 1)
					 && ((startOfObject(chunk)) == chunk)));

					/* For some reason the assertion is not compiled correctly */
					GIV(freeLists)[index] = ((next = longAt((void *)((chunk + BaseHeaderSize) + (0U << (shiftForWord()))))));
					if (next) {
						/* begin storePointer:ofFreeChunk:withValue: */
						assert(isFreeObject(next));
						longAtput((void *)((next + BaseHeaderSize) + (1U << (shiftForWord()))),0);
					}
					assert((bytesInBody(chunk)) == (index * (allocationUnit())));
					freeChunkWithBytesat((index * 8 /* allocationUnit */) - chunkBytes, (/* startOfObject: */
	((byteAt((void *)(chunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
		? chunk - BaseHeaderSize
		: chunk)) + chunkBytes);
					return chunk;
				}
				GIV(freeListsMask) -= 1ULL << index;
			}
		}

		/* now get desperate and use the first that'll fit.
		   Note that because the minimum free size is 16 bytes (2 * allocationUnit), to
		   leave room for the forwarding pointer/next free link, we can only break chunks
		   that are at least 16 bytes larger, hence start at initialIndex + 2. */
		index = initialIndex + 1;
		while ((((index += 1)) < 64 /* numFreeLists */)
		 && ((1ULL << index) <= GIV(freeListsMask))) {
			if (((GIV(freeListsMask) & (1ULL << index)) != 0)) {
				if ((chunk = GIV(freeLists)[index])) {
					assert(chunk == (startOfObject(chunk)));

					/* begin assertValidFreeObject: */
					assert(assertInnerValidFreeObject(chunk));
					assert(((bytesInBody(chunk)) == (index * (allocationUnit())))
					 && ((index > 1)
					 && ((startOfObject(chunk)) == chunk)));

					/* For some reason the assertion is not compiled correctly */
					GIV(freeLists)[index] = ((next = longAt((void *)((chunk + BaseHeaderSize) + (0U << (shiftForWord()))))));
					if (next) {
						/* begin storePointer:ofFreeChunk:withValue: */
						assert(isFreeObject(next));
						longAtput((void *)((next + BaseHeaderSize) + (1U << (shiftForWord()))),0);
					}
					assert((bytesInBody(chunk)) == (index * (allocationUnit())));
					freeChunkWithBytesat((index * 8 /* allocationUnit */) - chunkBytes, (/* startOfObject: */
	((byteAt((void *)(chunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
		? chunk - BaseHeaderSize
		: chunk)) + chunkBytes);
					return chunk;
				}
				GIV(freeListsMask) -= 1ULL << index;
			}
		}
	}

	/* Large chunk, or no space on small free lists.  Search the large chunk list.
	   Large chunk list organized as a tree, each node of which is a list of chunks
	   of the same size. Beneath the node are smaller and larger blocks.
	   When the search ends parent should hold the smallest chunk at least as
	   large as chunkBytes, or 0 if none. */
	parent = 0;
	child = GIV(freeLists)[0];
	while (child != 0) {
		/* begin assertValidFreeObject: */
		assert(assertInnerValidFreeObject(child));
		childBytes = bytesInBody(child);
		if (childBytes == chunkBytes) {
			chunk = longAt((void *)((child + BaseHeaderSize) + (0U << (shiftForWord()))));
			if (chunk) {
				/* begin assertValidFreeObject: */
				assert(assertInnerValidFreeObject(chunk));
				nextFreeChunk = longAt((void *)((chunk + BaseHeaderSize) + (0U << (shiftForWord()))));

				/* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
				/* begin storePointer:ofFreeChunk:withValue: */
				assert(isFreeObject(child));
				assert((nextFreeChunk == 0)
				 || (isFreeObject(nextFreeChunk)));
				longAtput((void *)((child + BaseHeaderSize) + (0U << (shiftForWord()))),nextFreeChunk);
				if (nextFreeChunk) {
					/* begin storePointer:ofFreeChunk:withValue: */
					assert(isFreeObject(nextFreeChunk));
					assert((child == 0)
					 || (isFreeObject(child)));
					longAtput((void *)((nextFreeChunk + BaseHeaderSize) + (1U << (shiftForWord()))),child);
				}
				return /* startOfObject: */
					((byteAt((void *)(chunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
						? chunk - BaseHeaderSize
						: chunk);
			}
			nodeBytes = childBytes;
			parent = child;

			/* break out of loop to remove interior node */
			child = 0;
		}
		else {
			if (childBytes <= (chunkBytes + 8 /* allocationUnit */)) {
				child = longAt((void *)((child + BaseHeaderSize) + (4U << (shiftForWord()))));
			}
			else {
				/* parent will be smallest node >= chunkBytes + allocationUnit */
				parent = child;
				nodeBytes = childBytes;
				child = longAt((void *)((child + BaseHeaderSize) + (3U << (shiftForWord()))));
			}
		}
	}
	if (!parent) {
		/* optimism was unfounded */
		GIV(totalFreeOldSpace) += chunkBytes;
		return null;
	}

	/* self printFreeChunk: parent */
	assert((nodeBytes == chunkBytes)
	 || (nodeBytes >= (chunkBytes + (2 * (allocationUnit())))));
	assert((bytesInBody(parent)) == nodeBytes);

	/* attempt to remove from list */
	chunk = longAt((void *)((parent + BaseHeaderSize) + (0U << (shiftForWord()))));
	if (chunk) {
		assert((chunkBytes == nodeBytes)
		 || ((chunkBytes + (allocationUnit())) < nodeBytes));
		nextFreeChunk = longAt((void *)((chunk + BaseHeaderSize) + (0U << (shiftForWord()))));

		/* begin setNextFreeChunkOf:withValue:isLilliputianSize: */
		/* begin storePointer:ofFreeChunk:withValue: */
		assert(isFreeObject(parent));
		assert((nextFreeChunk == 0)
		 || (isFreeObject(nextFreeChunk)));
		longAtput((void *)((parent + BaseHeaderSize) + (0U << (shiftForWord()))),nextFreeChunk);
		if (nextFreeChunk) {
			/* begin storePointer:ofFreeChunk:withValue: */
			assert(isFreeObject(nextFreeChunk));
			assert((parent == 0)
			 || (isFreeObject(parent)));
			longAtput((void *)((nextFreeChunk + BaseHeaderSize) + (1U << (shiftForWord()))),parent);
		}
		if (chunkBytes != nodeBytes) {
			freeChunkWithBytesat(nodeBytes - chunkBytes, (/* startOfObject: */
	((byteAt((void *)(chunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
		? chunk - BaseHeaderSize
		: chunk)) + chunkBytes);
		}
		return /* startOfObject: */
			((byteAt((void *)(chunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
				? chunk - BaseHeaderSize
				: chunk);
	}

	/* no list; remove the interior node */
	chunk = parent;
	unlinkSolitaryFreeTreeNode(chunk);

	/* if there's space left over, add the fragment back. */
	if (chunkBytes != nodeBytes) {
		freeChunkWithBytesat(nodeBytes - chunkBytes, (/* startOfObject: */
	((byteAt((void *)(chunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
		? chunk - BaseHeaderSize
		: chunk)) + chunkBytes);
	}
	return /* startOfObject: */
		((byteAt((void *)(chunk + (numSlotsFieldByteOffset())))) == (numSlotsMask())
			? chunk - BaseHeaderSize
			: chunk);
}