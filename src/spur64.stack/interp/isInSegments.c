/* Extracted from interp.c:49248 (function isInSegments). */

	/* SpurSegmentManager>>#isInSegments: */

static NoDbgRegParms sqInt
isInSegments(usqInt address)
{
    sqInt i;

	for (i = 0; i < numSegments; i += 1) {
		if (address < (((segments[i]).segStart))) {
			return 0;
		}
		if (address < ((((segments[i]).segSize)) + (((segments[i]).segStart)))) {
			return 1;
		}
	}
	return 0;
}