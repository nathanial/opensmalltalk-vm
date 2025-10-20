/* Extracted from B2DPlugin.c:3802 (function loadAndSubdivideBezierFromviatoisWide). */

static sqInt loadAndSubdivideBezierFromviatoisWide(int *point1, int *point2,
                                                   int *point3,
                                                   sqInt wideFlag) {
  int bz1;
  sqInt bz2;
  int index;
  sqInt index1;
  sqInt index2;

  /* begin allocateBezierStackEntry */
  /* begin wbStackPush: */
  /* begin allocateStackEntry: */
  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       6) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
  } else {
    /* wbTopPut: */
    workBuffer[GWBufferTop] = ((workBuffer[GWBufferTop]) - 6);
    /* end wbStackPush: */
  }
  bz1 = (workBuffer[GWSize]) - (workBuffer[GWBufferTop]);
  if (engineStopped) {
    return 0;
  }

  /* Load point1/point2/point3 on the top of the stack */

  /* #bzStartX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - bz1))] =
      (point1[0]);

  /* #bzStartY:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - bz1) + 1)] =
      (point1[1]);

  /* #bzViaX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - bz1) + 2)] =
      (point2[0]);

  /* #bzViaY:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - bz1) + 3)] =
      (point2[1]);

  /* #bzEndX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - bz1) + 4)] =
      (point3[0]);

  /* #bzEndY:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - bz1) + 5)] =
      (point3[1]);

  /* Now check if the bezier curve is monoton. If not, subdivide it. */
  index2 = (bz2 = subdivideToBeMonotoninX(bz1, wideFlag));
  for (index = bz1; index <= bz2; index += 6) {
    index1 = subdivideBezierFrom(index);
    if (index1 > index2) {
      index2 = index1;
    }
    if (engineStopped) {
      return 0;
    }
  }

  /* Return the number of segments */
  return index2 / 6;
}