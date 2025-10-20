/* Extracted from B2DPlugin.c:3036 (function findNextExternalFillFromAET). */

static sqInt findNextExternalFillFromAET(void) {
  sqInt fill;
  int leftEdge;
  int leftX;
  int rightEdge;
  int rightX;
  sqInt startX;
  int stopX;

  leftX = (rightX = workBuffer[GWFillMaxX]);
  while ((workBuffer[GWAETStart]) < (workBuffer[GWAETUsed])) {
    leftEdge = (rightEdge = aetBuffer[workBuffer[GWAETStart]]);

    /* TODO: We should check if leftX from last operation
       is  greater than leftX from next edge.
       Currently, we rely here on spanEndAA
       from the span buffer fill. */
    leftX = (rightX = objBuffer[leftEdge + GEXValue]);
    if (leftX >= (workBuffer[GWFillMaxX])) {
      return 0;
    }

    /* begin quickRemoveInvalidFillsAt: */
    if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
      while ((topRightX()) <= leftX) {
        hideFilldepth(topFill(), topDepth());
        if (!((workBuffer[GWSize]) - (workBuffer[GWBufferTop]))) {
          break;
        }
      }
    }
    /* end quickRemoveInvalidFillsAt: */

    /* Check if we need to draw the edge */
    if (((objBuffer[leftEdge + GEObjectType]) & GEPrimitiveTypeMask) &
        GEPrimitiveWide) {
      toggleWideFillOf(leftEdge);
    }
    if (!((objBuffer[leftEdge + GEObjectType]) & GEEdgeFillsInvalid)) {
      toggleFillsOf(leftEdge);
      if (engineStopped) {
        return 0;
      }
    }

    /* aetStartPut: */
    workBuffer[GWAETStart] = ((workBuffer[GWAETStart]) + 1);
    if ((workBuffer[GWAETStart]) < (workBuffer[GWAETUsed])) {
      rightEdge = aetBuffer[workBuffer[GWAETStart]];
      rightX = objBuffer[rightEdge + GEXValue];
      if (rightX >= (workBuffer[GWFillMinX])) {
        /* begin fillAllFrom:to: */
        /* begin topFill */
        if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
          fill =
              workBuffer[(workBuffer[GWBufferTop]) +
                         (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                          3 /* stackFillEntryLength */)];
        } else {
          fill = 0;
        }
        startX = leftX;

        /* begin topRightX */
        if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
          stopX =
              workBuffer[(workBuffer[GWBufferTop]) +
                         ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                           3 /* stackFillEntryLength */) +
                          2)];
        } else {
          stopX = 999999999;
        }
        int fillError = 0;
        while (stopX < rightX && !fillError) {
          /* begin topFill */
          if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
            fill =
                workBuffer[(workBuffer[GWBufferTop]) +
                           (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                            3 /* stackFillEntryLength */)];
          } else {
            fill = 0;
          }
          if (fill) {
            if (fillSpanfromto(fill, startX, stopX)) {
              fillError = 1;
              break;
            }
          }

          /* begin quickRemoveInvalidFillsAt: */
          if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
            while ((topRightX()) <= stopX) {
              hideFilldepth(topFill(), topDepth());
              if (!((workBuffer[GWSize]) - (workBuffer[GWBufferTop]))) {
                break;
              }
            }
          }
          /* end quickRemoveInvalidFillsAt: */

          startX = stopX;

          /* begin topRightX */
          if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
            stopX = workBuffer[(workBuffer[GWBufferTop]) +
                               ((((workBuffer[GWSize]) -
                                  (workBuffer[GWBufferTop])) -
                                 3 /* stackFillEntryLength */) +
                                2)];
          } else {
            stopX = 999999999;
          }
        }

        if (!fillError) {
          /* begin topFill */
          if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
            fill =
                workBuffer[(workBuffer[GWBufferTop]) +
                           (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                            3 /* stackFillEntryLength */)];
          } else {
            fill = 0;
          }
          if (fill) {
            fillSpanfromto(fill, startX, rightX);
          }
        }
        /* end fillAllFrom:to: */
      }
    }
  }

  /* Note: Due to pre-clipping we may have to draw remaining stuff with the last
   * fill */
  if (rightX < (workBuffer[GWFillMaxX])) {
    /* begin fillAllFrom:to: */
    /* begin topFill */
    if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
      fill = workBuffer[(workBuffer[GWBufferTop]) +
                        (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                         3 /* stackFillEntryLength */)];
    } else {
      fill = 0;
    }
    startX = rightX;

    /* begin topRightX */
    if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
      stopX = workBuffer[(workBuffer[GWBufferTop]) +
                         ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                           3 /* stackFillEntryLength */) +
                          2)];
    } else {
      stopX = 999999999;
    }
    int fillError = 0;
    while (stopX < (workBuffer[GWFillMaxX]) && !fillError) {
      /* begin topFill */
      if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
        fill = workBuffer[(workBuffer[GWBufferTop]) +
                          (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                           3 /* stackFillEntryLength */)];
      } else {
        fill = 0;
      }
      if (fill) {
        if (fillSpanfromto(fill, startX, stopX)) {
          fillError = 1;
          break;
        }
      }

      /* begin quickRemoveInvalidFillsAt: */
      if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
        while ((topRightX()) <= stopX) {
          hideFilldepth(topFill(), topDepth());
          if (!((workBuffer[GWSize]) - (workBuffer[GWBufferTop]))) {
            break;
          }
        }
      }
      /* end quickRemoveInvalidFillsAt: */

      startX = stopX;

      /* begin topRightX */
      if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
        stopX =
            workBuffer[(workBuffer[GWBufferTop]) +
                       ((((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                         3 /* stackFillEntryLength */) +
                        2)];
      } else {
        stopX = 999999999;
      }
    }

    if (!fillError) {
      /* begin topFill */
      if ((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) {
        fill = workBuffer[(workBuffer[GWBufferTop]) +
                          (((workBuffer[GWSize]) - (workBuffer[GWBufferTop])) -
                           3 /* stackFillEntryLength */)];
      } else {
        fill = 0;
      }
      if (fill) {
        fillSpanfromto(fill, startX, workBuffer[GWFillMaxX]);
      }
    }
    /* end fillAllFrom:to: */
  }
  return 0;
}