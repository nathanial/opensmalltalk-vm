/* Extracted from interp.c:49646 (function shrinkObjectMemory). */

/*	Answer if any shrinkage was achieved. */

	/* SpurSegmentManager>>#shrinkObjectMemory: */

static NoDbgRegParms int
shrinkObjectMemory(usqInt delta)
{
    sqInt address;
    usqInt chunkBytes;
    usqInt currentEnd;
    sqInt deltaSqInt;
    SpurSegmentInfo *emptySeg;
    sqInt freeChunk;
    sqInt i;
    sqInt iSqInt;
    sqInt j;
    sqInt k;
    usqInt numSlots;
    SpurSegmentInfo *seg;
    SpurSegmentInfo *segInfo;
    usqInt shrinkage;

	/* begin checkFreeSpace: */
	assert(bitsSetInFreeSpaceMaskForAllFreeLists());
	assert(totalFreeOldSpace == (totalFreeListBytes()));
	if (((checkForLeaks & (GCCheckFreeSpace | GCCheckFreeSpace)) == (GCCheckFreeSpace | GCCheckFreeSpace))) {
		runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
	}
	shrinkage = delta;
	while (1) {
		/* begin findEmptySegNearestInSizeTo: */
		emptySeg = null;
		deltaSqInt = shrinkage;
		for (k = 0; k < numSegments; k += 1) {
			seg = (&(segments[k]));
			if (isEmptySegment(seg)) {
				if (emptySeg) {
					if ((shrinkage >= (((seg->segSize)) * 0.75))
					 && ((SQABS(((sqInt) (((seg->segSize)) - shrinkage)))) < deltaSqInt)) {
						emptySeg = seg;
						deltaSqInt = SQABS(((sqInt) (((seg->segSize)) - shrinkage)));
					}
				}
				else {
					emptySeg = seg;
				}
			}
		}
		if ((!emptySeg)
		 || (((emptySeg->segSize)) > shrinkage)) {
			/* begin checkFreeSpace: */
			assert(bitsSetInFreeSpaceMaskForAllFreeLists());
			assert(totalFreeOldSpace == (totalFreeListBytes()));
			if (((checkForLeaks & (GCCheckFreeSpace | GCCheckFreeSpace)) == (GCCheckFreeSpace | GCCheckFreeSpace))) {
				runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
			}
			return shrinkage < delta;
		}
		shrinkage -= (emptySeg->segSize);
		address = (emptySeg->segStart);

		/* begin objectStartingAt: */
		numSlots = byteAt((void *)(address + (numSlotsFieldByteOffset())));
		freeChunk = (numSlots == (numSlotsMask())
					? address + BaseHeaderSize
					: address);

		/* begin detachFreeObject: */
		chunkBytes = bytesInBody(freeChunk);
		totalFreeOldSpace -= chunkBytes;
		unlinkFreeChunkchunkBytes(freeChunk, chunkBytes);

		/* begin removeSegment: */
		i = 0;
		for (iSqInt = 0; iSqInt < numSegments; iSqInt += 1) {
			if (((emptySeg->segStart)) == (((segments[iSqInt]).segStart))) {
				i = iSqInt;
				goto l1;
			}
		}
		error("segment not found");
		/* end indexOfSegment: */
l1:
		assert(i > 0);
		totalHeapSizeIncludingBridges -= (emptySeg->segSize);
		sqDeallocateMemorySegmentAtOfSize(((void *)((emptySeg->segStart))), (emptySeg->segSize));
		for (j = i; j <= (numSegments - 2); j += 1) {
			segments[j] = (segments[j + 1]);
		}
		numSegments -= 1;
		bridgeFromto((&(segments[i - 1])), (i <= (numSegments - 1)
				? (&(segments[i]))
				: 0));
		segInfo = (&(segments[numSegments - 1]));

		/* begin setLastSegment: */
		currentEnd = (((segInfo->segSize)) + ((segInfo->segStart))) - (2 * BaseHeaderSize);
		if (currentEnd <= endOfMemory) {
			endOfMemory = currentEnd;
			if (freeOldSpaceStart > currentEnd) {
				freeOldSpaceStart = currentEnd;
			}
		}
	}
	return 0;
}