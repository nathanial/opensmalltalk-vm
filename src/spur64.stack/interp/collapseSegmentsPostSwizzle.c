/* Extracted from interp.c:49179 (function collapseSegmentsPostSwizzle). */

static void
collapseSegmentsPostSwizzle(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    SpurSegmentInfo *cascade0;

	GIV(canSwizzle) = 0;
	GIV(numSegments) = 1;

	/* begin computeTotalHeapSizeIncludingBridges */
	GIV(totalHeapSizeIncludingBridges) = GIV(endOfMemory) - GIV(oldSpaceStart);
	cascade0 = (&(GIV(segments)[0]));
	(cascade0->segStart = GIV(oldSpaceStart));
	(cascade0->segSize = GIV(totalHeapSizeIncludingBridges));

	/* finally plant a bridge at the end of the coalesced segment and cut back the
	   manager's notion of the end of memory to immediately before the bridge. */
	assert(isSegmentBridge(bridgeAt(0)));
	assert((numSlotsOfAny(bridgeAt(0))) == 0);
}