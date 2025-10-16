/* Extracted from interp.c:49575 (function segmentContainingObj). */

SpurSegmentInfo *
segmentContainingObj(sqInt objOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt high;
    sqInt low;
    sqInt mid;
    SpurSegmentInfo *seg;

	low = 0;
	mid = GIV(numSegments) / 2;
	high = GIV(numSegments) - 1;
	do {
		seg = (&(GIV(segments)[mid]));
		if (oopisGreaterThanOrEqualTo(objOop, (seg->segStart))) {
			if (mid == high) {
				return (oopisLessThan(objOop, ((seg->segSize)) + ((seg->segStart)))
						? seg
						: 0);
			}
			else {
				low = mid;
				mid = ((mid + high) + 1) / 2;
			}
		}
		else {
			high = mid - 1;
			mid = (low + mid) / 2;
		}
	} while(low <= high);
	return null;
}