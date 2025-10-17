/* Extracted from interp.c:49611 (function segmentOverlap). */

/*	Answers true if a segment overlaps with another one. */

	/* SpurSegmentManager>>#segmentOverlap */

static sqInt
segmentOverlap(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt endi;
    usqInt endj;
    sqInt i;
    sqInt j;
    usqInt starti;
    usqInt startj;

	for (i = 0; i < GIV(numSegments); i += 1) {
		starti = ((GIV(segments)[i]).segStart);
		endi = (((GIV(segments)[i]).segSize)) + (((GIV(segments)[i]).segStart));
		for (j = 0; j < GIV(numSegments); j += 1) {
			startj = ((GIV(segments)[j]).segStart);
			endj = (((GIV(segments)[j]).segSize)) + (((GIV(segments)[j]).segStart));
			if (!(i == j)) {
				if (!((starti < startj)
					 || (starti >= endj))) {
					return 1;
				}
				if (!((endi <= startj)
					 || (endi >= endj))) {
					return 1;
				}
			}
		}
	}
	return 0;
}