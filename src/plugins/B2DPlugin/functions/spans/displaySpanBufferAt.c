/* Extracted from B2DPlugin.c:1480 (function displaySpanBufferAt). */

static sqInt displaySpanBufferAt(sqInt y) {
  int targetX0;
  int targetX1;
  sqInt targetY;

  /* self aaLevelGet > 1 ifTrue:[self adjustAALevel]. */
  targetX0 = ((usqInt)((workBuffer[GWSpanStart]))) >> (workBuffer[GWAAShift]);
  if (targetX0 < (workBuffer[GWClipMinX])) {
    targetX0 = workBuffer[GWClipMinX];
  }
  targetX1 =
      ((usqInt)((((workBuffer[GWSpanEnd]) + (workBuffer[GWAALevel])) - 1))) >>
      (workBuffer[GWAAShift]);
  if (targetX1 > (workBuffer[GWClipMaxX])) {
    targetX1 = workBuffer[GWClipMaxX];
  }
  targetY = ((usqInt)(y)) >> (workBuffer[GWAAShift]);
  if ((targetY < (workBuffer[GWClipMinY])) ||
      ((targetY >= (workBuffer[GWClipMaxY])) ||
       ((targetX1 < (workBuffer[GWClipMinX])) ||
        (targetX0 >= (workBuffer[GWClipMaxX]))))) {
    return 0;
  }
  copyBitsFromtoat(targetX0, targetX1, targetY);
  return 0;
}