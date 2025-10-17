/* Extracted from interp.c:49575 (function segmentContainingObj). */

/*	Answer the segment containing an object. This is mostly for assert
	checking, but
	variations on the incremental GC may use it in anger. Binary search is (of
	course) marginally slower than linear search for a single segment (e.g. in
	a 720k object heap,
	67.1ms vs 61.3ms, or 9.5% slower to derive the segment containing every
	old space
	entity), but usefully faster for many segments (e.g. 92.7ms vs 116ms, or
	20% faster
	in the same heap extended with enough large arrays to require 11 segments;
	and this
	is pessimal; there are fewer objects at high addresses since the large
	arrays are there). */

	/* SpurSegmentManager>>#segmentContainingObj: */

SpurSegmentInfo *
segmentContainingObj(sqInt objOop)
{
    sqInt high;
    sqInt low;
    sqInt mid;
    SpurSegmentInfo *seg;

	low = 0;
	mid = numSegments / 2;
	high = numSegments - 1;
	do {
		seg = (&(segments[mid]));
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