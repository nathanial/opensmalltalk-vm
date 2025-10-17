/* Extracted from interp.c:49509 (function restorePostSnapshot). */

/*	Restore all shortened segments to their proper size, re-freeing the
	trailing space.
 */

	/* SpurSegmentManager>>#restorePostSnapshot */

static void
restorePostSnapshot(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt address;
    sqInt bytes;
    usqInt freeChunk;
    sqInt i;
    sqInt newEndOfMemory;
    SpurSegmentInfo *seg;


	/* Set endOfMemory first, to avoid assert fails in freeChunkWithBytes:at:. */
	seg = (&(GIV(segments)[GIV(numSegments) - 1]));
	if ((seg->lastFreeObject)) {
		newEndOfMemory = (((seg->savedSegSize)) + ((seg->segStart))) - (2 * BaseHeaderSize);

		/* begin setEndOfMemory: */
		GIV(endOfMemory) = newEndOfMemory;
		if (GIV(freeOldSpaceStart) > newEndOfMemory) {
			GIV(freeOldSpaceStart) = newEndOfMemory;
		}
	}
	else {
		assert((GIV(endOfMemory)) == ((segLimit(seg)) - (bridgeSize())));
	}
	for (i = (GIV(numSegments) - 1); i >= 0; i += -1) {
		seg = (&(GIV(segments)[i]));
		if ((freeChunk = (seg->lastFreeObject))) {
			address = (((seg->segSize)) + ((seg->segStart))) - (2 * BaseHeaderSize);
			(seg->segSize = (seg->savedSegSize));
			bridgeFromto(seg, (i < (GIV(numSegments) - 1)
					? (&(GIV(segments)[i + 1]))
					: 0));
			bytes = ((((seg->segSize)) + ((seg->segStart))) - address) - (2 * BaseHeaderSize);

			/* begin addFreeChunkWithBytes:at: */
			GIV(totalFreeOldSpace) += bytes;
			freeChunkWithBytesat(bytes, address);
		}
	}
	checkSegments();

	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(GIV(totalFreeOldSpace) == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCModeFull)) == (GCCheckFreeSpace | GCModeFull))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}
}