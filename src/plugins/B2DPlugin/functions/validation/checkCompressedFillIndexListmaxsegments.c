/* Extracted from B2DPlugin.c:828 (function checkCompressedFillIndexListmaxsegments). */

static sqInt checkCompressedFillIndexListmaxsegments(sqInt fillList,
                                                     sqInt maxIndex,
                                                     sqInt nSegs) {
  int *fillPtr;
  sqInt i;
  sqInt length;
  sqInt nFills;
  sqInt runLength;
  int runValue;

  length = slotSizeOf(fillList);
  fillPtr = firstIndexableField(fillList);
  nFills = 0;
  for (i = 0; i < length; i += 1) {
    runLength = (((usqInt)(((((int *)fillPtr))[i]))) >> 16);
    runValue = ((((int *)fillPtr))[i]) & 0xFFFF;
    if (!((runValue >= 0) && (runValue <= maxIndex))) {
      return 0;
    }
    nFills += runLength;
  }
  return nFills == nSegs;
}