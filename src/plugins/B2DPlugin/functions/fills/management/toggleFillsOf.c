/* Extracted from B2DPlugin.c:10668 (function toggleFillsOf). */

static sqInt toggleFillsOf(sqInt edge) {
  sqInt depth;
  int fillIndex;

  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       6 /* (stackFillEntryLength * 2) */) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return null;
  }
  depth = ((sqInt)((usqInt)((objBuffer[edge + GEZValue])) << 1));
  fillIndex = objBuffer[edge + GEFillIndexLeft];
  if (fillIndex) {
    toggleFilldepthrightX(fillIndex, depth, 999999999);
  }
  fillIndex = objBuffer[edge + GEFillIndexRight];
  if (fillIndex) {
    toggleFilldepthrightX(fillIndex, depth, 999999999);
  }

  /* begin quickRemoveInvalidFillsAt: */
  if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
    while ((topRightX()) <= (objBuffer[edge + GEXValue])) {
      hideFilldepth(topFill(), topDepth());
      if (!((workBuffer[GWSize]) - (workBuffer[GWBufferTop]))) {
        break;
      }
    }
  }
  /* end quickRemoveInvalidFillsAt: */
  return 0;
}