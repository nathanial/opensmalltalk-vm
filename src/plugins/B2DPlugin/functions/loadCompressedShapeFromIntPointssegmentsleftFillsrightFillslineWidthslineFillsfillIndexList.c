/* Extracted from B2DPlugin.c:4037 (function loadCompressedShapeFromIntPointssegmentsleftFillsrightFillslineWidthslineFillsfillIndexList). */

static sqInt
loadCompressedShapeFromIntPointssegmentsleftFillsrightFillslineWidthslineFillsfillIndexList(
    int *points, sqInt nSegments, int *leftFills, int *rightFills,
    int *lineWidths, int *lineFills, int *fillIndexList) {
  int *dstPoint1;
  sqInt i;
  sqInt index;
  sqInt leftLength;
  sqInt leftRun;
  sqInt leftValue;
  sqInt lineFillLength;
  sqInt lineFillRun;
  unsigned int lineFillValue;
  int *point;
  sqInt rightLength;
  sqInt rightRun;
  sqInt rightValue;
  sqInt segs;
  float *transform;
  sqInt widthLength;
  sqInt widthRun;
  sqInt widthValue;
  sqInt x;
  int x0;
  int x1;
  int x2;
  sqInt y;
  int y0;
  int y1;
  int y2;

  if (!nSegments) {
    return 0;
  }

  /* Initialize run length encodings */
  leftRun = (rightRun = (widthRun = (lineFillRun = -1)));
  leftLength = (rightLength = (widthLength = (lineFillLength = 1)));
  leftValue = (rightValue = (widthValue = (lineFillValue = 0)));
  for (i = 1; i <= nSegments; i += 1) {
    if (((leftLength -= 1)) <= 0) {
      leftRun += 1;
      leftLength = (((usqInt)(((((int *)leftFills))[leftRun]))) >> 16);
      leftValue = ((((int *)leftFills))[leftRun]) & 0xFFFF;
      if (leftValue) {
        leftValue = fillIndexList[leftValue - 1];
        leftValue = transformColor(leftValue);
        if (engineStopped) {
          return null;
        }
      }
    }
    if (((rightLength -= 1)) <= 0) {
      rightRun += 1;
      rightLength = (((usqInt)(((((int *)rightFills))[rightRun]))) >> 16);
      rightValue = ((((int *)rightFills))[rightRun]) & 0xFFFF;
      if (rightValue) {
        rightValue = fillIndexList[rightValue - 1];
        rightValue = transformColor(rightValue);
      }
    }
    if (((widthLength -= 1)) <= 0) {
      widthRun += 1;
      widthLength = (((usqInt)(((((int *)lineWidths))[widthRun]))) >> 16);
      widthValue = ((((int *)lineWidths))[widthRun]) & 0xFFFF;
      if (widthValue) {
        widthValue = transformWidth(widthValue);
      }
    }
    if (((lineFillLength -= 1)) <= 0) {
      lineFillRun += 1;
      lineFillLength = (((usqInt)(((((int *)lineFills))[lineFillRun]))) >> 16);
      lineFillValue = ((((int *)lineFills))[lineFillRun]) & 0xFFFF;
      if (lineFillValue) {
        lineFillValue = fillIndexList[lineFillValue - 1];
      }
    }

    /* begin
     * loadCompressedSegment:fromIntPoints:leftFill:rightFill:lineWidth:lineColor:
     */
    if (!((leftValue == rightValue) &&
          ((widthValue == 0) || (lineFillValue == 0)))) {
      /* 3 points with x/y each */
      index = (i - 1) * 6;
      x0 = points[index];
      y0 = points[index + 1];
      x1 = points[index + 2];
      y1 = points[index + 3];
      x2 = points[index + 4];
      y2 = points[index + 5];

      /* Briefly check if can represent the bezier as a line */
      if (((x0 == x1) && (y0 == y1)) || ((x1 == x2) && (y1 == y2))) {
        if (!((x0 == x2) && (y0 == y2))) {
          (*((int *)(workBuffer + GWPoint1)) = x0);
          (((int *)(workBuffer + GWPoint1)))[1] = y0;
          (*((int *)(workBuffer + GWPoint2)) = x2);
          (((int *)(workBuffer + GWPoint2)))[1] = y2;

          /* begin transformPoints: */
          {
            point = ((int *)(workBuffer + GWPoint1));

            /* begin transformPoint: */
            if (workBuffer[GWHasEdgeTransform]) {
              /* begin transformPoint:into: */
              dstPoint1 = ((int *)point);
              transform = ((float *)(workBuffer + GWEdgeTransform));
              x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                             ((transform[1]) *
                              (((double)((((int *)point))[1]))))) +
                            (transform[2])) *
                           (((double)(workBuffer[GWAALevel])))));
              y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                             ((transform[4]) *
                              (((double)((((int *)point))[1]))))) +
                            (transform[5])) *
                           (((double)(workBuffer[GWAALevel])))));
              dstPoint1[0] = x;
              dstPoint1[1] = y;
            } else {
              point[0] = (((point[0]) + (workBuffer[GWDestOffsetX])) *
                          (workBuffer[GWAALevel]));
              point[1] = (((point[1]) + (workBuffer[GWDestOffsetY])) *
                          (workBuffer[GWAALevel]));
            }
          }
          {
            point = ((int *)(workBuffer + GWPoint2));

            /* begin transformPoint: */
            if (workBuffer[GWHasEdgeTransform]) {
              /* begin transformPoint:into: */
              dstPoint1 = ((int *)point);
              transform = ((float *)(workBuffer + GWEdgeTransform));
              x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                             ((transform[1]) *
                              (((double)((((int *)point))[1]))))) +
                            (transform[2])) *
                           (((double)(workBuffer[GWAALevel])))));
              y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                             ((transform[4]) *
                              (((double)((((int *)point))[1]))))) +
                            (transform[5])) *
                           (((double)(workBuffer[GWAALevel])))));
              dstPoint1[0] = x;
              dstPoint1[1] = y;
            } else {
              point[0] = (((point[0]) + (workBuffer[GWDestOffsetX])) *
                          (workBuffer[GWAALevel]));
              point[1] = (((point[1]) + (workBuffer[GWDestOffsetY])) *
                          (workBuffer[GWAALevel]));
            }
          }

          loadWideLinefromtolineFillleftFillrightFill(
              widthValue, ((int *)(workBuffer + GWPoint1)),
              ((int *)(workBuffer + GWPoint2)), lineFillValue, leftValue,
              rightValue);
        }
      } else {
        /* We can use a line from x0/y0 to x2/y2
           Need bezier curve
           Load and transform points */
        (*((int *)(workBuffer + GWPoint1)) = x0);
        (((int *)(workBuffer + GWPoint1)))[1] = y0;
        (*((int *)(workBuffer + GWPoint2)) = x1);
        (((int *)(workBuffer + GWPoint2)))[1] = y1;
        (*((int *)(workBuffer + GWPoint3)) = x2);
        (((int *)(workBuffer + GWPoint3)))[1] = y2;

        /* begin transformPoints: */
        {
          point = ((int *)(workBuffer + GWPoint1));

          /* begin transformPoint: */
          if (workBuffer[GWHasEdgeTransform]) {
            /* begin transformPoint:into: */
            dstPoint1 = ((int *)point);
            transform = ((float *)(workBuffer + GWEdgeTransform));
            x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                           ((transform[1]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[2])) *
                         (((double)(workBuffer[GWAALevel])))));
            y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                           ((transform[4]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[5])) *
                         (((double)(workBuffer[GWAALevel])))));
            dstPoint1[0] = x;
            dstPoint1[1] = y;
          } else {
            point[0] = (((point[0]) + (workBuffer[GWDestOffsetX])) *
                        (workBuffer[GWAALevel]));
            point[1] = (((point[1]) + (workBuffer[GWDestOffsetY])) *
                        (workBuffer[GWAALevel]));
          }
        }
        {
          point = ((int *)(workBuffer + GWPoint2));

          /* begin transformPoint: */
          if (workBuffer[GWHasEdgeTransform]) {
            /* begin transformPoint:into: */
            dstPoint1 = ((int *)point);
            transform = ((float *)(workBuffer + GWEdgeTransform));
            x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                           ((transform[1]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[2])) *
                         (((double)(workBuffer[GWAALevel])))));
            y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                           ((transform[4]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[5])) *
                         (((double)(workBuffer[GWAALevel])))));
            dstPoint1[0] = x;
            dstPoint1[1] = y;
          } else {
            point[0] = (((point[0]) + (workBuffer[GWDestOffsetX])) *
                        (workBuffer[GWAALevel]));
            point[1] = (((point[1]) + (workBuffer[GWDestOffsetY])) *
                        (workBuffer[GWAALevel]));
          }
        }
        {
          point = ((int *)(workBuffer + GWPoint3));

          /* begin transformPoint: */
          if (workBuffer[GWHasEdgeTransform]) {
            /* begin transformPoint:into: */
            dstPoint1 = ((int *)point);
            transform = ((float *)(workBuffer + GWEdgeTransform));
            x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                           ((transform[1]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[2])) *
                         (((double)(workBuffer[GWAALevel])))));
            y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                           ((transform[4]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[5])) *
                         (((double)(workBuffer[GWAALevel])))));
            dstPoint1[0] = x;
            dstPoint1[1] = y;
          } else {
            point[0] = (((point[0]) + (workBuffer[GWDestOffsetX])) *
                        (workBuffer[GWAALevel]));
            point[1] = (((point[1]) + (workBuffer[GWDestOffsetY])) *
                        (workBuffer[GWAALevel]));
          }
        }

        segs = loadAndSubdivideBezierFromviatoisWide(
            ((int *)(workBuffer + GWPoint1)), ((int *)(workBuffer + GWPoint2)),
            ((int *)(workBuffer + GWPoint3)),
            (widthValue != 0) && (lineFillValue != 0));
        if (!engineStopped) {
          loadWideBezierlineFillleftFillrightFilln(widthValue, lineFillValue,
                                                   leftValue, rightValue, segs);
        }
      }
    }
    /* end
     * loadCompressedSegment:fromIntPoints:leftFill:rightFill:lineWidth:lineColor:
     */

    if (engineStopped) {
      return null;
    }
  }
  return 0;
}