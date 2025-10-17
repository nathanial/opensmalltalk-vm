/* Extracted from interp.c:49441 (function readHeapFromImageFiledataBytes). */

/*	Read numBytes of image data from f into memory at
   memoryBaseForImageRead. Answer the number of bytes written. In addition, read
   each segment, build up the segment info for swizzling, while eliminating the
   bridge objects at the end of each segment that specify the distance to and
   the size of the subsequent segment.  */

/* SpurSegmentManager>>#readHeapFromImageFile:dataBytes: */

static sqInt readHeapFromImageFiledataBytes(sqImageFile f,
                                                          sqInt numBytes) {
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

  /* segment sizes include the two-header-word bridge at the end of each
   * segment. */
  numSegments = (totalBytesRead = 0);

  /* N.B. still must be adjusted by oldBaseAddr. */
  oldBase = 0;
  newBase = oldSpaceStart;
  nextSegmentSize = firstSegmentSize;
  bridgehead = (firstSegmentSize + oldSpaceStart) - (2 * BaseHeaderSize);
  while (1) {
    segInfo = (&(segments[numSegments]));
    (segInfo->segStart = oldBase);
    (segInfo->segSize = nextSegmentSize);
    (segInfo->swizzle = newBase - oldBase);
    bytesRead = sqImageFileRead(pointerForOop(newBase), sizeof(char),
                                nextSegmentSize, f);
    if (bytesRead > 0) {
      totalBytesRead += bytesRead;
    }
    if (bytesRead != nextSegmentSize) {
      return totalBytesRead;
    }
    if (((numSegments += 1)) >= numSegInfos) {
      allocateOrExtendSegmentInfos();
    }
    bridge = bridgehead + BaseHeaderSize;
    bridgeSpan =
        (byteAt((void *)(bridgehead + (numSlotsFieldByteOffset())))
             ? BytesPerOop *
                   (((((usqInt)(((sqInt)((usqInt)((longAt((
                                             void *)(bridge - BaseHeaderSize))))
                                         << 8)))))) >>
                    8)
             : 0);
    oldBase = (oldBase + nextSegmentSize) + bridgeSpan;
    newBase = (newBase + nextSegmentSize) - (2 * BaseHeaderSize);
    nextSegmentSize = ((usqInt)(long64At((void *)(bridge))));
    if (!(nextSegmentSize != 0))
      break;
    bridgehead = (bridgehead - (2 * BaseHeaderSize)) + nextSegmentSize;
  }

  /* newBase should point just past the last bridge. all others should have been
   * eliminated. */
  assert((newBase - (oldSpaceStart)) ==
         (totalBytesRead - (numSegments * (bridgeSize()))));

  /* set freeOldSpaceStart now for adjustAllOopsBy: */

  /* begin setFreeOldSpaceStart: */
  freeOldSpaceStart = newBase;

  /* we're done. nil firstSegmentSize for a subsequent snapshot. */
  firstSegmentSize = null;
  return totalBytesRead;
}