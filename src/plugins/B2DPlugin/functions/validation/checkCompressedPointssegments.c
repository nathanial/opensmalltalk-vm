/* Extracted from B2DPlugin.c:904 (function checkCompressedPointssegments). */

static sqInt checkCompressedPointssegments(sqInt points, sqInt nSegments) {
  sqInt pSize;

  if (!(isWords(points))) {
    return 0;
  }
  pSize = slotSizeOf(points);

  /* The points must be either in PointArray format or ShortPointArray format.
     Also, we currently handle only quadratic segments (e.g., 3 points each) and
     thus either pSize = nSegments * 3,		for ShortPointArrays or, pSize =
     nSegments * 6,		for PointArrays */
  if (!((pSize == (nSegments * 3)) || (pSize == (nSegments * 6)))) {
    return 0;
  }
  return 1;
}