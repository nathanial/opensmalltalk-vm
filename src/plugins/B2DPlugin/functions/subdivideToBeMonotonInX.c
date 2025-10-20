/* Extracted from B2DPlugin.c:10554 (function subdivideToBeMonotonInX). */

static sqInt subdivideToBeMonotonInX(sqInt index) {
  int denom;
  int dx1;
  int dx2;
  int endX;
  int num;
  int startX;
  int viaX;

  startX = workBuffer[(workBuffer[GWBufferTop]) +
                      ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                        index))];
  viaX =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  2)];
  endX =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  4)];
  dx1 = viaX - startX;
  dx2 = endX - viaX;
  if ((dx1 * dx2) >= 0) {
    return index;
  }

  /* incrementStat:by: */
  workBuffer[GWBezierMonotonSubdivisions] =
      ((workBuffer[GWBezierMonotonSubdivisions]) + 1);

  /* Compute split value */
  denom = dx2 - dx1;
  num = dx1;
  if (num < 0) {
    num = 0 - num;
  }
  if (denom < 0) {
    denom = 0 - denom;
  }
  return computeBeziersplitAt(index, (((double)num)) / (((double)denom)));
}