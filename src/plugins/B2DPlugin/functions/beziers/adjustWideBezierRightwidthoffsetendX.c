/* Extracted from B2DPlugin.c:668 (function adjustWideBezierRightwidthoffsetendX). */

static sqInt adjustWideBezierRightwidthoffsetendX(sqInt bezier, sqInt lineWidth,
                                                  sqInt lineOffset,
                                                  sqInt endX) {
  int lastX;
  int lastY;

  ((objBuffer + bezier) + GBUpdateData)[GBUpdateX] =
      ((((objBuffer + bezier) + GBUpdateData)[GBUpdateX]) +
       (lineOffset * 0x100));

  /* Set the lastX/Y value of the second curve lineWidth pixels right/down
     Set lineWidth-lineOffset pixels left */
  lastX = ((objBuffer + bezier) + GBWideUpdateData)[GBUpdateX];
  ((objBuffer + bezier) + GBWideUpdateData)[GBUpdateX] =
      (lastX - ((lineWidth - lineOffset) * 0x100));
  lastY = ((objBuffer + bezier) + GBWideUpdateData)[GBUpdateY];

  /* Set lineWidth pixels down */
  ((objBuffer + bezier) + GBWideUpdateData)[GBUpdateY] =
      (lastY + (lineWidth * 0x100));

  /* Record the last X value */

  /* #bezierFinalXOf:put: #obj:at:put: */
  objBuffer[bezier + GBFinalX] = ((endX - lineOffset) + lineWidth);
  return 0;
}