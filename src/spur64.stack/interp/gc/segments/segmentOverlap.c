/* Extracted from interp.c:49611 (function segmentOverlap). */

/*	Answers true if a segment overlaps with another one. */

/* SpurSegmentManager>>#segmentOverlap */

static sqInt segmentOverlap(void) {
  usqInt endi;
  usqInt endj;
  sqInt i;
  sqInt j;
  usqInt starti;
  usqInt startj;

  for (i = 0; i < numSegments; i += 1) {
    starti = ((segments[i]).segStart);
    endi = (((segments[i]).segSize)) + (((segments[i]).segStart));
    for (j = 0; j < numSegments; j += 1) {
      startj = ((segments[j]).segStart);
      endj = (((segments[j]).segSize)) + (((segments[j]).segStart));
      if (!(i == j)) {
        if (!((starti < startj) || (starti >= endj))) {
          return 1;
        }
        if (!((endi <= startj) || (endi >= endj))) {
          return 1;
        }
      }
    }
  }
  return 0;
}