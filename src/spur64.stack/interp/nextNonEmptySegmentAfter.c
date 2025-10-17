/* Extracted from interp.c:49289 (function nextNonEmptySegmentAfter). */

/*	Answer the the next non-empty segment or nil. The size of a segment
	includes that of its bridge. A segment containing just a free object and a
	bridge will still
	have a size of manager bridgeSize after shortening it in
	prepareForSnapshot. 
 */

	/* SpurSegmentManager>>#nextNonEmptySegmentAfter: */

static NoDbgRegParms SpurSegmentInfo *
nextNonEmptySegmentAfter(sqInt i)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt nextx;

	nextx = i;
	while (1) {
		if (((nextx += 1)) >= numSegments) {
			return null;
		}
		if ((((segments[nextx]).segSize)) > (2 * BaseHeaderSize)) {
			return (&(segments[nextx]));
		}
	}
	return 0;
}