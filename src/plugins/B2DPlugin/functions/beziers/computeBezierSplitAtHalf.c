/* Extracted from B2DPlugin.c:1070 (function computeBezierSplitAtHalf). */

static int computeBezierSplitAtHalf(sqInt index) {
  int endX;
  int endY;
  int leftViaX;
  int leftViaY;
  int newIndex;
  int rightViaX;
  int rightViaY;
  int sharedX;
  int sharedY;
  int startX;
  int startY;
  int viaX;
  int viaY;

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
  }
  /* end wbStackPush: */
  newIndex = (workBuffer[GWSize]) - (workBuffer[GWBufferTop]);
  if (engineStopped) {
    return 0;
  }
  leftViaX =
      (startX =
           workBuffer[(workBuffer[GWBufferTop]) +
                      ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                        index))]);
  leftViaY =
      (startY =
           workBuffer[(workBuffer[GWBufferTop]) +
                      ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                        index) +
                       1)]);
  rightViaX =
      (viaX = workBuffer[(workBuffer[GWBufferTop]) +
                         ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                           index) +
                          2)]);
  rightViaY =
      (viaY = workBuffer[(workBuffer[GWBufferTop]) +
                         ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                           index) +
                          3)]);
  endX =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  4)];
  endY =
      workBuffer[(workBuffer[GWBufferTop]) +
                 ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
                  5)];

  /* Compute intermediate points */
  leftViaX += (viaX - startX) / 2;
  leftViaY += (viaY - startY) / 2;
  sharedX = (rightViaX += (endX - viaX) / 2);
  sharedY = (rightViaY += (endY - viaY) / 2);

  /* Compute new shared point */
  sharedX += (leftViaX - rightViaX) / 2;
  sharedY += (leftViaY - rightViaY) / 2;

  /* Store the first part back */

  /* #bzViaX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
              2)] = leftViaX;

  /* #bzViaY:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
              3)] = leftViaY;

  /* #bzEndX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
              4)] = sharedX;

  /* #bzEndY:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - index) +
              5)] = sharedY;

  /* Store the second point back */

  /* #bzStartX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
               newIndex))] = sharedX;

  /* #bzStartY:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - newIndex) +
              1)] = sharedY;

  /* #bzViaX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - newIndex) +
              2)] = rightViaX;

  /* #bzViaY:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - newIndex) +
              3)] = rightViaY;

  /* #bzEndX:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - newIndex) +
              4)] = endX;

  /* #bzEndY:put: #wbStackValue:put: */
  workBuffer[(workBuffer[GWBufferTop]) +
             ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) - newIndex) +
              5)] = endY;
  return newIndex;
}