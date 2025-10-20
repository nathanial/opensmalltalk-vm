/* Extracted from B2DPlugin.c:10035 (function stepToFirstWideBezierInat). */

static sqInt stepToFirstWideBezierInat(sqInt bezier, sqInt yValue) {
  int endX;
  sqInt i;
  sqInt lineOffset;
  int lineWidth;
  int nLines;
  int startY;
  int xDir;
  sqInt yEntry;
  sqInt yExit;

  /* Get some values */
  lineWidth = objBuffer[bezier + GBWideExtent];
  lineOffset = lineWidth / 2;

  /* Compute the incremental values of the bezier */
  endX = objBuffer[bezier + GBEndX];
  startY = objBuffer[bezier + GEYValue];
  stepToFirstBezierInat(bezier, startY);
  nLines = objBuffer[bezier + GENumLines];

  /* Copy the incremental update data */
  for (i = 0; i <= 5; i += 1) {
    ((objBuffer + bezier) + GBWideUpdateData)[i] =
        (((objBuffer + bezier) + GBUpdateData)[i]);
  }

  /* Compute primary x direction of curve (e.g., 1: left to right; -1: right to
   * left). */
  xDir = ((objBuffer + bezier) + GBUpdateData)[GBUpdateDX];
  if (!xDir) {
    xDir = ((objBuffer + bezier) + GBUpdateData)[GBUpdateDDX];
  }
  if (xDir >= 0) {
    xDir = 1;
  } else {
    xDir = -1;
  }

  /* Adjust the curve to start/end at the right position */
  if (xDir < 0) {
    adjustWideBezierLeftwidthoffsetendX(bezier, lineWidth, lineOffset, endX);
  } else {
    adjustWideBezierRightwidthoffsetendX(bezier, lineWidth, lineOffset, endX);
  }

  /* Adjust the last value for horizontal lines */
  if (!nLines) {
    ((objBuffer + bezier) + GBUpdateData)[GBUpdateX] =
        ((objBuffer[bezier + GBFinalX]) * 0x100);
  }

  /* Adjust the number of lines to include the lineWidth */

  /* #edgeNumLinesOf:put: #obj:at:put: */
  objBuffer[bezier + GENumLines] = (nLines + lineWidth);

  /* Compute the points where we have to turn on/off the fills */

  /* turned on at lineOffset */
  yEntry = 0;

  /* turned off at zero */
  yExit = (0 - nLines) - lineOffset;

  /* #wideBezierEntryOf:put: #obj:at:put: */
  objBuffer[bezier + GBWideEntry] = yEntry;

  /* #wideBezierExitOf:put: #obj:at:put: */
  objBuffer[bezier + GBWideExit] = yExit;

  /* Turn the fills on/off as necessary */
  if ((yEntry >= lineOffset) && (yExit < 0)) {
    /* #edgeFillsValidate: #objectTypeOf:put: #obj:at:put: */
    objBuffer[bezier + GEObjectType] =
        (((objBuffer[bezier + GEObjectType]) & GEPrimitiveTypeMask) &
         ((unsigned int)~GEEdgeFillsInvalid));
  } else {
    /* #edgeFillsInvalidate: #objectTypeOf:put: #obj:at:put: */
    objBuffer[bezier + GEObjectType] =
        (((objBuffer[bezier + GEObjectType]) & GEPrimitiveTypeMask) |
         GEEdgeFillsInvalid);
  }
  computeFinalWideBezierValueswidth(bezier, lineWidth);

  /* And step to the first scan line */
  if (!(startY == yValue)) {
    for (i = startY; i < yValue; i += 1) {
      stepToNextWideBezierInat(bezier, i);
    }

    /* Adjust number of lines remaining */

    /* #edgeNumLinesOf:put: #obj:at:put: */
    objBuffer[bezier + GENumLines] =
        ((objBuffer[bezier + GENumLines]) - (yValue - startY));
  }
  return 0;
}