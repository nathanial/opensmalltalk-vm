/* Extracted from B2DPlugin.c:638 (function adjustWideBezierLeftwidthoffsetendX). */

static sqInt adjustWideBezierLeftwidthoffsetendX(sqInt bezier, sqInt lineWidth,
                                                 sqInt lineOffset, sqInt endX) {
  int lastX;
  int lastY;

  ((objBuffer + bezier) + GBUpdateData)[GBUpdateX] =
      ((((objBuffer + bezier) + GBUpdateData)[GBUpdateX]) -
       (lineOffset * 0x100));

  /* Set the lastX/Y value of the second curve lineWidth pixels right/down */
  lastX = ((objBuffer + bezier) + GBWideUpdateData)[GBUpdateX];
  ((objBuffer + bezier) + GBWideUpdateData)[GBUpdateX] =
      (lastX + ((lineWidth - lineOffset) * 0x100));

  /* Set lineWidth pixels down */
  lastY = ((objBuffer + bezier) + GBWideUpdateData)[GBUpdateY];
  ((objBuffer + bezier) + GBWideUpdateData)[GBUpdateY] =
      (lastY + (lineWidth * 0x100));

  /* Record the last X value */

  /* #bezierFinalXOf:put: #obj:at:put: */
  objBuffer[bezier + GBFinalX] = (endX - lineOffset);
  return 0;
}