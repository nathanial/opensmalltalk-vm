/* Extracted from B2DPlugin.c:2825 (function fillSortsbefore). */

static int fillSortsbefore(sqInt fillEntry1, sqInt fillEntry2) {
  int diff;

  /* First check the depth value */
  diff = (workBuffer[(workBuffer[GWBufferTop]) + (fillEntry1 + 1)]) -
         (workBuffer[(workBuffer[GWBufferTop]) + (fillEntry2 + 1)]);
  if (diff) {
    return diff > 0;
  }

  /* See the class comment for aetScanningProblems */
  return (((unsigned)(workBuffer[(workBuffer[GWBufferTop]) + fillEntry1]))) <
         (((unsigned)(workBuffer[(workBuffer[GWBufferTop]) + fillEntry2])));
}