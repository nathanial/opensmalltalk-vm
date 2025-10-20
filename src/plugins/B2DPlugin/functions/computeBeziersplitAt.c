/* Extracted from B2DPlugin.c:1207 (function computeBeziersplitAt). */

static int computeBeziersplitAt(sqInt index, double param) {
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
  sharedX = (leftViaX += ((sqInt)((((double)(viaX - startX))) * param)));
  sharedY = (leftViaY += ((sqInt)((((double)(viaY - startY))) * param)));
  rightViaX += ((sqInt)((((double)(endX - viaX))) * param));
  rightViaY += ((sqInt)((((double)(endY - viaY))) * param));

  /* Compute new shared point */
  sharedX += ((sqInt)((((double)(rightViaX - leftViaX))) * param));
  sharedY += ((sqInt)((((double)(rightViaY - leftViaY))) * param));

  /* Check the new via points */

  /* begin assureValue:between:and: */
  if (startY > sharedY) {
    if (leftViaY > startY) {
      leftViaY = startY;
    } else if (leftViaY < sharedY) {
      leftViaY = sharedY;
    }
  } else {
    if (leftViaY < startY) {
      leftViaY = startY;
    } else if (leftViaY > sharedY) {
      leftViaY = sharedY;
    }
  }
  /* end assureValue:between:and: */

  /* begin assureValue:between:and: */
  if (sharedY > endY) {
    if (rightViaY > sharedY) {
      rightViaY = sharedY;
    } else if (rightViaY < endY) {
      rightViaY = endY;
    }
  } else {
    if (rightViaY < sharedY) {
      rightViaY = sharedY;
    } else if (rightViaY > endY) {
      rightViaY = endY;
    }
  }
  /* end assureValue:between:and: */

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