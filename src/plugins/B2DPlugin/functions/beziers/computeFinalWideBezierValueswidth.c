/* Extracted from B2DPlugin.c:1375 (function computeFinalWideBezierValueswidth). */

static sqInt computeFinalWideBezierValueswidth(sqInt bezier, sqInt lineWidth) {
  int leftX;
  int rightX;
  int temp;

  leftX = (((objBuffer + bezier) + GBUpdateData)[GBUpdateX]) / 0x100;
  rightX = (((objBuffer + bezier) + GBWideUpdateData)[GBUpdateX]) / 0x100;
  if (leftX > rightX) {
    temp = leftX;
    leftX = rightX;
    rightX = temp;
  }

  /* #edgeXValueOf:put: #obj:at:put: */
  objBuffer[bezier + GEXValue] = leftX;
  if ((rightX - leftX) > lineWidth) {
    /* #wideBezierWidthOf:put: #obj:at:put: */
    objBuffer[bezier + GBWideWidth] = (rightX - leftX);
  } else {
    /* #wideBezierWidthOf:put: #obj:at:put: */
    objBuffer[bezier + GBWideWidth] = lineWidth;
  }
  return 0;
}