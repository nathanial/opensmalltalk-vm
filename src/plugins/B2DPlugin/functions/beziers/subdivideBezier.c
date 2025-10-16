/* Extracted from B2DPlugin.c:10493 (function subdivideBezier). */

static sqInt subdivideBezier(sqInt index) {
  int deltaX;
  int deltaY;
  int endX;
  int endY;
  int startX;
  int startY;

  startY =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  1)];
  endY =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  5)];

  /* If the receiver is horizontal, don't do anything */
  if (endY == startY) {
    return index;
  }

  /* TODO: If the curve can be represented as a line, then do so
     If the height of the curve exceeds 256 pixels, subdivide
     (forward differencing is numerically not very stable) */
  deltaY = endY - startY;
  if (deltaY < 0) {
    deltaY = 0 - deltaY;
  }
  if (deltaY > 0xFF) {
    /* incrementStat:by: */
    workBuffer[GWBezierHeightSubdivisions] =
        ((workBuffer[GWBezierHeightSubdivisions]) + 1);
    return computeBezierSplitAtHalf(index);
  }

  /* Check if the incremental values could possibly overflow the scaled integer
   * range */
  startX = workBuffer[(workBuffer[GWBufferTop]) +
                      ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                        index))];
  endX =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  4)];
  deltaX = endX - startX;
  if (deltaX < 0) {
    deltaX = 0 - deltaX;
  }
  if ((deltaY * 32) < deltaX) {
    /* incrementStat:by: */
    workBuffer[GWBezierOverflowSubdivisions] =
        ((workBuffer[GWBezierOverflowSubdivisions]) + 1);
    return computeBezierSplitAtHalf(index);
  }
  return index;
}