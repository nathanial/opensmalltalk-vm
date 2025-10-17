/* Extracted from interp.c:49768 (function totalBytesInSegments). */

/*	This ``slow'' count is for asserts only. */

	/* SpurSegmentManager>>#totalBytesInSegments */

static usqInt
totalBytesInSegments(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    usqInt total;

	total = 0;
	for (i = 0; i < GIV(numSegments); i += 1) {
		total += ((GIV(segments)[i]).segSize);
	}
	return total;
}