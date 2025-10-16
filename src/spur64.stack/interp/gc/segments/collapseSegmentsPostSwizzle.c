/* Extracted from interp.c:49179 (function collapseSegmentsPostSwizzle). */

/*	The image has been loaded, old segments reconstructed, and the heap
        swizzled into a single contiguous segment. Collapse the segments into
   one. */

/* SpurSegmentManager>>#collapseSegmentsPostSwizzle */

static void collapseSegmentsPostSwizzle(void) {
  SpurSegmentInfo *cascade0;

  canSwizzle = 0;
  numSegments = 1;

  /* begin computeTotalHeapSizeIncludingBridges */
  totalHeapSizeIncludingBridges = endOfMemory - oldSpaceStart;
  cascade0 = (&(segments[0]));
  (cascade0->segStart = oldSpaceStart);
  (cascade0->segSize = totalHeapSizeIncludingBridges);

  /* finally plant a bridge at the end of the coalesced segment and cut back the
     manager's notion of the end of memory to immediately before the bridge. */
  assert(isSegmentBridge(bridgeAt(0)));
  assert((numSlotsOfAny(bridgeAt(0))) == 0);
}