/* Extracted from interp.c:49768 (function totalBytesInSegments). */

/*	This ``slow'' count is for asserts only. */

	/* SpurSegmentManager>>#totalBytesInSegments */

static usqInt
totalBytesInSegments(void)
{
    sqInt i;
    usqInt total;

	total = 0;
	for (i = 0; i < numSegments; i += 1) {
		total += ((segments[i]).segSize);
	}
	return total;
}