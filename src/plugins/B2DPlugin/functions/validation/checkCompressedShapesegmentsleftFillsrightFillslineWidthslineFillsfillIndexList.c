/* Extracted from B2DPlugin.c:927 (function checkCompressedShapesegmentsleftFillsrightFillslineWidthslineFillsfillIndexList). */

static sqInt
checkCompressedShapesegmentsleftFillsrightFillslineWidthslineFillsfillIndexList(
    sqInt points, sqInt nSegments, sqInt leftFills, sqInt rightFills,
    sqInt lineWidths, sqInt lineFills, sqInt fillIndexList) {
  sqInt maxFillIndex;

  if (!(checkCompressedPointssegments(points, nSegments))) {
    return 0;
  }
  if (!(checkCompressedFills(fillIndexList))) {
    return 0;
  }
  maxFillIndex = slotSizeOf(fillIndexList);
  if (!(checkCompressedFillIndexListmaxsegments(leftFills, maxFillIndex,
                                                nSegments))) {
    return 0;
  }
  if (!(checkCompressedFillIndexListmaxsegments(rightFills, maxFillIndex,
                                                nSegments))) {
    return 0;
  }
  if (!(checkCompressedFillIndexListmaxsegments(lineFills, maxFillIndex,
                                                nSegments))) {
    return 0;
  }
  if (!(checkCompressedLineWidthssegments(lineWidths, nSegments))) {
    return 0;
  }
  return 1;
}