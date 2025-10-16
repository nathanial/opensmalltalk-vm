/* Extracted from interp.c:49289 (function nextNonEmptySegmentAfter). */

static NoDbgRegParms SpurSegmentInfo *
nextNonEmptySegmentAfter(sqInt i)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt nextx;

	nextx = i;
	while (1) {
		if (((nextx += 1)) >= GIV(numSegments)) {
			return null;
		}
		if ((((GIV(segments)[nextx]).segSize)) > (2 * BaseHeaderSize)) {
			return (&(GIV(segments)[nextx]));
		}
	}
	return 0;
}