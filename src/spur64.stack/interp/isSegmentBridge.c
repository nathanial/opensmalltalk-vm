/* Extracted from interp.c:37615 (function isSegmentBridge). */

static NoDbgRegParms int
isSegmentBridge(sqInt objOop)
{
	return ((longAt((void *)(objOop))) & (classIndexMask())) == (segmentBridgePun());
}