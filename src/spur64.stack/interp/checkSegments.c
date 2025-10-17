/* Extracted from interp.c:49161 (function checkSegments). */

	/* SpurSegmentManager>>#checkSegments */

static void
checkSegments(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;

	assert(numSegments >= 1);
	for (i = 0; i < numSegments; i += 1) {
		assert(addressCouldBeObj(((segments[i]).segStart)));
		assert(isValidSegmentBridge(bridgeAt(i)));
	}
	assert(((segLimit(&segments[numSegments - 1])) - (bridgeSize())) == (endOfMemory));
}