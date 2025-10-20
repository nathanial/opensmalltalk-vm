/* Extracted from B2DPlugin.c:10599 (function subdivideToBeMonotonInY). */

static sqInt subdivideToBeMonotonInY(sqInt index) {
  int denom;
  int dy1;
  int dy2;
  int endY;
  int num;
  int startY;
  int viaY;

  startY =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  1)];
  viaY =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  3)];
  endY =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  5)];
  dy1 = viaY - startY;
  dy2 = endY - viaY;
  if ((dy1 * dy2) >= 0) {
    return index;
  }

  /* incrementStat:by: */
  workBuffer[GWBezierMonotonSubdivisions] =
      ((workBuffer[GWBezierMonotonSubdivisions]) + 1);

  /* Compute split value */
  denom = dy2 - dy1;
  num = dy1;
  if (num < 0) {
    num = 0 - num;
  }
  if (denom < 0) {
    denom = 0 - denom;
  }
  return computeBeziersplitAt(index, (((double)num)) / (((double)denom)));
}