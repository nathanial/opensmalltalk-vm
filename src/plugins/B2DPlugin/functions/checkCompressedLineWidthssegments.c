/* Extracted from B2DPlugin.c:883 (function checkCompressedLineWidthssegments). */

static int checkCompressedLineWidthssegments(sqInt lineWidthList,
                                             sqInt nSegments) {
  sqInt i;
  sqInt length;
  sqInt nItems;
  int *ptr;
  sqInt runLength;

  length = slotSizeOf(lineWidthList);
  ptr = firstIndexableField(lineWidthList);
  nItems = 0;
  for (i = 0; i < length; i += 1) {
    runLength = (((usqInt)(((((int *)ptr))[i]))) >> 16);
    nItems += runLength;
  }
  return nItems == nSegments;
}