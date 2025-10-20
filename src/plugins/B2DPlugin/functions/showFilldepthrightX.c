/* Extracted from B2DPlugin.c:9711 (function showFilldepthrightX). */

static sqInt showFilldepthrightX(sqInt fillIndex, sqInt depth, sqInt rightX) {
  sqInt value;

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
    return null;
  }

  /* wbTopPut: */
  workBuffer[GWBufferTop] =
      ((workBuffer[GWBufferTop]) - 3 /* stackFillEntryLength */);

  /* #stackFillValue:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop])] = fillIndex;

  /* #stackFillDepth:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) + 1] = depth;

  /* #stackFillRightX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) + 2] = rightX;
  if (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) ==
      3 /* stackFillEntryLength */) {
    return null;
  }
  if (fillSortsbefore(0, ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                             3 /* stackFillEntryLength */)) {
    value = workBuffer[(workBuffer[GWBufferTop]) +
                       (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                        3 /* stackFillEntryLength */)];

    /* begin stackFillValue:put: */
    /* wbStackValue:put: */
    workBuffer[(workBuffer[GWBufferTop])] = value;
    value = workBuffer[(workBuffer[GWBufferTop]) +
                       ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                         3 /* stackFillEntryLength */) +
                        1)];

    /* begin stackFillDepth:put: */
    /* wbStackValue:put: */
    workBuffer[(workBuffer[GWBufferTop]) + 1] = value;
    value = workBuffer[(workBuffer[GWBufferTop]) +
                       ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                         3 /* stackFillEntryLength */) +
                        2)];

    /* begin stackFillRightX:put: */
    /* wbStackValue:put: */
    workBuffer[(workBuffer[GWBufferTop]) + 2] = value;

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
  return 0;
}