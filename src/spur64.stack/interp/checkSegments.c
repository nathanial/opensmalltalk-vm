/* Extracted from interp.c:49161 (function checkSegments). */

static void
checkSegments(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;

	assert(GIV(numSegments) >= 1);
	for (i = 0; i < GIV(numSegments); i += 1) {
		assert(addressCouldBeObj(((GIV(segments)[i]).segStart)));
		assert(isValidSegmentBridge(bridgeAt(i)));
	}
	assert(((segLimit(&GIV(segments)[GIV(numSegments) - 1])) - (bridgeSize())) == (GIV(endOfMemory)));
}