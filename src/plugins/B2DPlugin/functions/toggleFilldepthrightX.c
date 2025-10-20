/* Extracted from B2DPlugin.c:10708 (function toggleFilldepthrightX). */

static sqInt toggleFilldepthrightX(sqInt fillIndex, sqInt depth, sqInt rightX) {
  sqInt hidden;

  if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
    hidden = hideFilldepth(fillIndex, depth);
    if (!hidden) {
      showFilldepthrightX(fillIndex, depth, rightX);
    }
  } else {
    /* begin allocateStackFillEntry */
    /* begin wbStackPush: */
    /* begin allocateStackEntry: */
    /* begin needAvailableSpace: */
    if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
          (workBuffer[GWAETUsed])) +
         3 /* stackFillEntryLength */) > (workBuffer[GWBufferTop])) {
      /* begin stopBecauseOf: */
      /* stopReasonPut: */
      workBuffer[GWStopReason] = GErrorNoMoreSpace;
      engineStopped = 1;
    } else {
      /* wbTopPut: */
      workBuffer[GWBufferTop] =
          ((workBuffer[GWBufferTop]) - 3 /* stackFillEntryLength */);

      /* #topFillValuePut: #stackFillValue:put: #wbStackValue:put: */
      workBuffer[(workBuffer[GWBufferTop]) +
                 (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                  3 /* stackFillEntryLength */)] = fillIndex;

      /* #topFillDepthPut: #stackFillDepth:put: #wbStackValue:put: */
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                   3 /* stackFillEntryLength */) +
                  1)] = depth;

      /* #topFillRightXPut: #stackFillRightX:put: #wbStackValue:put: */
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                   3 /* stackFillEntryLength */) +
                  2)] = rightX;
    }
  }
  return 0;
}