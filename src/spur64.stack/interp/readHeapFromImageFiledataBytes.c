/* Extracted from interp.c:49441 (function readHeapFromImageFiledataBytes). */

static NoDbgRegParms sqInt
readHeapFromImageFiledataBytes(sqImageFile f, sqInt numBytes)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt bridge;
    sqInt bridgehead;
    usqInt bridgeSpan;
    sqInt bytesRead;
    usqInt newBase;
    usqInt nextSegmentSize;
    sqInt oldBase;
    SpurSegmentInfo *segInfo;
    sqInt totalBytesRead;

	allocateOrExtendSegmentInfos();

	/* segment sizes include the two-header-word bridge at the end of each segment. */
	GIV(numSegments) = (totalBytesRead = 0);

	/* N.B. still must be adjusted by oldBaseAddr. */
	oldBase = 0;
	newBase = GIV(oldSpaceStart);
	nextSegmentSize = GIV(firstSegmentSize);
	bridgehead = (GIV(firstSegmentSize) + GIV(oldSpaceStart)) - (2 * BaseHeaderSize);
	while (1) {
		segInfo = (&(GIV(segments)[GIV(numSegments)]));
		(segInfo->segStart = oldBase);
		(segInfo->segSize = nextSegmentSize);
		(segInfo->swizzle = newBase - oldBase);
		bytesRead = sqImageFileRead(pointerForOop(newBase), sizeof(char), nextSegmentSize, f);
		if (bytesRead > 0) {
			totalBytesRead += bytesRead;
		}
		if (bytesRead != nextSegmentSize) {
			return totalBytesRead;
		}
		if (((GIV(numSegments) += 1)) >= GIV(numSegInfos)) {
			allocateOrExtendSegmentInfos();
		}
		bridge = bridgehead + BaseHeaderSize;
		bridgeSpan = (byteAt((void *)(bridgehead + (numSlotsFieldByteOffset())))
					? BytesPerOop * (((((usqInt)(((sqInt)((usqInt)((longAt((void *)(bridge - BaseHeaderSize)))) << 8)))))) >> 8)
					: 0);
		oldBase = (oldBase + nextSegmentSize) + bridgeSpan;
		newBase = (newBase + nextSegmentSize) - (2 * BaseHeaderSize);
		nextSegmentSize = ((usqInt)(long64At((void *)(bridge))));
		if (!(nextSegmentSize != 0)) break;
		bridgehead = (bridgehead - (2 * BaseHeaderSize)) + nextSegmentSize;
	}

	/* newBase should point just past the last bridge. all others should have been eliminated. */
	assert((newBase - (GIV(oldSpaceStart))) == (totalBytesRead - (GIV(numSegments) * (bridgeSize()))));

	/* set freeOldSpaceStart now for adjustAllOopsBy: */

	/* begin setFreeOldSpaceStart: */
	GIV(freeOldSpaceStart) = newBase;

	/* we're done. nil firstSegmentSize for a subsequent snapshot. */
	GIV(firstSegmentSize) = null;
	return totalBytesRead;
}