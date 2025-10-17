/* Extracted from interp.c:37615 (function isSegmentBridge). */

/*	Maybe this should be in SpurSegmentManager only */

	/* SpurMemoryManager>>#isSegmentBridge: */

static NoDbgRegParms int
isSegmentBridge(sqInt objOop)
{
	return ((longAt((void *)(objOop))) & (classIndexMask())) == (segmentBridgePun());
}