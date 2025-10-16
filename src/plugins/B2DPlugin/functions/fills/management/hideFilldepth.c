/* Extracted from B2DPlugin.c:3469 (function hideFilldepth). */

static sqInt hideFilldepth(sqInt fillIndex, sqInt depth) {
  sqInt index;
  int newDepth;
  int newRightX;
  int newTop;
  sqInt newTopIndex;
  sqInt value;

  index = findStackFilldepth(fillIndex, depth);
  if (index == -1) {
    return 0;
  }
  if (!index) {
    /* begin freeStackFillEntry */
    /* begin wbStackPop: */
    /* wbTopPut: */
    workBuffer[GWBufferTop] =
        ((workBuffer[GWBufferTop]) + 3 /* stackFillEntryLength */);
    return 1;
  }

  /* Fill is visible - replace it with the last entry on the stack */

  /* #stackFillValue:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) + index] =
      (workBuffer[(workBuffer[GWBufferTop])]);

  /* #stackFillDepth:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) + (index + 1)] =
      (workBuffer[(workBuffer[GWBufferTop]) + 1]);

  /* #stackFillRightX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) + (index + 2)] =
      (workBuffer[(workBuffer[GWBufferTop]) + 2]);

  /* begin freeStackFillEntry */
  /* begin wbStackPop: */
  /* wbTopPut: */
  workBuffer[GWBufferTop] =
      ((workBuffer[GWBufferTop]) + 3 /* stackFillEntryLength */);
  if (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) <=
      3 /* stackFillEntryLength */) {
    return 1;
  }
  newTopIndex = 0;
  index = 3 /* stackFillEntryLength */;
  while (index < ((workBuffer[GWSize]) - (workBuffer[GWBufferTop]))) {
    if (fillSortsbefore(index, newTopIndex)) {
      newTopIndex = index;
    }
    index += 3 /* stackFillEntryLength */;
  }
  if ((newTopIndex + 3 /* stackFillEntryLength */) ==
      ((workBuffer[GWSize]) - (workBuffer[GWBufferTop]))) {
    return 1;
  }
  newTop = workBuffer[(workBuffer[GWBufferTop]) + newTopIndex];
  value = workBuffer[(workBuffer[GWBufferTop]) +
                     (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                      3 /* stackFillEntryLength */)];

  /* begin stackFillValue:put: */
  /* wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) + newTopIndex] = value;

  /* #topFillValuePut: #stackFillValue:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
              3 /* stackFillEntryLength */)] = newTop;
  newDepth = workBuffer[(workBuffer[GWBufferTop]) + (newTopIndex + 1)];
  value = workBuffer[(workBuffer[GWBufferTop]) +
                     ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                       3 /* stackFillEntryLength */) +
                      1)];

  /* begin stackFillDepth:put: */
  /* wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) + (newTopIndex + 1)] = value;

  /* #topFillDepthPut: #stackFillDepth:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
               3 /* stackFillEntryLength */) +
              1)] = newDepth;
  newRightX = workBuffer[(workBuffer[GWBufferTop]) + (newTopIndex + 2)];
  value = workBuffer[(workBuffer[GWBufferTop]) +
                     ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                       3 /* stackFillEntryLength */) +
                      2)];

  /* begin stackFillRightX:put: */
  /* wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) + (newTopIndex + 2)] = value;

  /* #topFillRightXPut: #stackFillRightX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
               3 /* stackFillEntryLength */) +
              2)] = newRightX;
  return 1;
}