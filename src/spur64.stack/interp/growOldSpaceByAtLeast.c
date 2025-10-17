/* Extracted from interp.c:36051 (function growOldSpaceByAtLeast). */

/*	Attempt to grow memory by at least minAmmount.
        Answer the size of the new segment, or nil if the attempt failed. */

/* SpurMemoryManager>>#growOldSpaceByAtLeast: */

static NoDbgRegParms usqInt growOldSpaceByAtLeast(sqInt minAmmount) {
  sqInt address;
  sqInt ammount;
  sqInt bytes;
  sqInt headroom;
  sqInt interval;
  SpurSegmentInfo *segInfo;
  sqInt start;
  usqInt total;

  /* statGrowMemory counts attempts, not successes. */
  /* we need to include overhead for a new object header plus the segment
   * bridge. */
  statGrowMemory += 1;
  ammount = minAmmount + ((BaseHeaderSize * 2) + (2 * BaseHeaderSize));

  /* round up to the nearest power of two. */
  ammount = 1ULL << (highBit(ammount - 1));

  /* and grow by at least growHeadroom. */
  ammount = ((ammount < growHeadroom) ? growHeadroom : ammount);

  /* Now apply the maxOldSpaceSize limit, if one is in effect. */
  if (maxOldSpaceSize > 0) {
    /* begin totalOldSpaceSize */
    assert((totalBytesInSegments()) == totalHeapSizeIncludingBridges);
    total = totalHeapSizeIncludingBridges;
    if (total >= maxOldSpaceSize) {
      needGCFlag = 1;
      return null;
    }
    headroom = maxOldSpaceSize - total;
    if (headroom < ammount) {
      if (headroom <
          (minAmmount + ((BaseHeaderSize * 2) + (2 * BaseHeaderSize)))) {
        needGCFlag = 1;
        return null;
      }
      ammount = headroom;
    }
  }
  start = ioUTCMicrosecondsNow();
  if ((segInfo = addSegmentOfSize(ammount))) {
    /* begin assimilateNewSegment: */
    if ((((segInfo->segSize)) + ((segInfo->segStart))) >= endOfMemory) {
      freeOldSpaceStart =
          (endOfMemory = (((segInfo->segSize)) + ((segInfo->segStart))) -
                         (2 * BaseHeaderSize));
    }

    /* and add the new free chunk to the free list; done here
       instead of in assimilateNewSegment: for the assert */
    bytes = ((segInfo->segSize)) - (2 * BaseHeaderSize);
    address = (segInfo->segStart);

    /* begin addFreeChunkWithBytes:at: */
    totalFreeOldSpace += bytes;
    freeChunkWithBytesat(bytes, address);
    assert((addressAfter(objectStartingAt((segInfo->segStart)))) ==
           ((segLimit(segInfo)) - (bridgeSize())));

    /* begin checkFreeSpace: */
    assert(bitsSetInFreeSpaceMaskForAllFreeLists());
    assert(totalFreeOldSpace == (totalFreeListBytes()));
    if (((checkForLeaks & (GCCheckFreeSpace | GCCheckFreeSpace)) ==
         (GCCheckFreeSpace | GCCheckFreeSpace))) {
      runLeakCheckerForFreeSpaceignoring(GCCheckFreeSpace, null);
    }
    checkSegments();
    interval = (ioUTCMicrosecondsNow()) - start;
    if (interval > statMaxAllocSegmentTime) {
      statMaxAllocSegmentTime = interval;
    }
    return (segInfo->segSize);
  } else {
    needGCFlag = 1;
    return null;
  }
}