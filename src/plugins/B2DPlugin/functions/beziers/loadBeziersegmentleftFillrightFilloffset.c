/* Extracted from B2DPlugin.c:3883 (function loadBeziersegmentleftFillrightFilloffset). */

static sqInt loadBeziersegmentleftFillrightFilloffset(sqInt bezier, sqInt index,
                                                      sqInt leftFillIndex,
                                                      sqInt rightFillIndex,
                                                      sqInt yOffset) {
  if ((workBuffer[(workBuffer[GWBufferTop]) +
                  ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                    index) +
                   5)]) >=
      (workBuffer[(workBuffer[GWBufferTop]) +
                  ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                    index) +
                   1)])) {
    /* #edgeXValueOf:put: #obj:at:put: */
    objBuffer[bezier + GEXValue] =
        (workBuffer[(workBuffer[GWBufferTop]) +
                    ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                      index))]);

    /* #edgeYValueOf:put: #obj:at:put: */
    objBuffer[bezier + GEYValue] =
        ((workBuffer[(workBuffer[GWBufferTop]) +
                     ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                       index) +
                      1)]) -
         yOffset);

    /* #bezierViaXOf:put: #obj:at:put: */
    objBuffer[bezier + GBViaX] =
        (workBuffer[(workBuffer[GWBufferTop]) +
                    ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                      index) +
                     2)]);

    /* #bezierViaYOf:put: #obj:at:put: */
    objBuffer[bezier + GBViaY] =
        ((workBuffer[(workBuffer[GWBufferTop]) +
                     ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                       index) +
                      3)]) -
         yOffset);

    /* #bezierEndXOf:put: #obj:at:put: */
    objBuffer[bezier + GBEndX] =
        (workBuffer[(workBuffer[GWBufferTop]) +
                    ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                      index) +
                     4)]);

    /* #bezierEndYOf:put: #obj:at:put: */
    objBuffer[bezier + GBEndY] =
        ((workBuffer[(workBuffer[GWBufferTop]) +
                     ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                       index) +
                      5)]) -
         yOffset);
  } else {
    /* #edgeXValueOf:put: #obj:at:put: */
    objBuffer[bezier + GEXValue] =
        (workBuffer[(workBuffer[GWBufferTop]) +
                    ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                      index) +
                     4)]);

    /* #edgeYValueOf:put: #obj:at:put: */
    objBuffer[bezier + GEYValue] =
        ((workBuffer[(workBuffer[GWBufferTop]) +
                     ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                       index) +
                      5)]) -
         yOffset);

    /* #bezierViaXOf:put: #obj:at:put: */
    objBuffer[bezier + GBViaX] =
        (workBuffer[(workBuffer[GWBufferTop]) +
                    ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                      index) +
                     2)]);

    /* #bezierViaYOf:put: #obj:at:put: */
    objBuffer[bezier + GBViaY] =
        ((workBuffer[(workBuffer[GWBufferTop]) +
                     ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                       index) +
                      3)]) -
         yOffset);

    /* #bezierEndXOf:put: #obj:at:put: */
    objBuffer[bezier + GBEndX] =
        (workBuffer[(workBuffer[GWBufferTop]) +
                    ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                      index))]);

    /* #bezierEndYOf:put: #obj:at:put: */
    objBuffer[bezier + GBEndY] =
        ((workBuffer[(workBuffer[GWBufferTop]) +
                     ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                       index) +
                      1)]) -
         yOffset);
  }

  /* Top to bottom */

  /* #edgeZValueOf:put: #obj:at:put: */
  objBuffer[bezier + GEZValue] = (workBuffer[GWCurrentZ]);

  /* #edgeLeftFillOf:put: #obj:at:put: */
  objBuffer[bezier + GEFillIndexLeft] = leftFillIndex;

  /* #edgeRightFillOf:put: #obj:at:put: */
  objBuffer[bezier + GEFillIndexRight] = rightFillIndex;
  return 0;
}