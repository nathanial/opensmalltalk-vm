/* Extracted from B2DPlugin.c:5911 (function primitiveAddBezierShape). */

EXPORT(sqInt)
primitiveAddBezierShape(void) {
  int *dstPoint1;
  sqInt failureCode;
  usqInt fillIndex;
  sqInt i;
  sqInt index;
  sqInt length;
  usqInt lineFill;
  sqInt lineWidth;
  sqInt nSegments;
  int *point;
  sqInt points;
  sqInt pointsIsArray;
  sqInt pointsIsShorts;
  sqInt segSize;
  sqInt segs;
  float *transform;
  sqInt x;
  short x0;
  int x0Int;
  short x1;
  int x1Int;
  short x2;
  int x2Int;
  sqInt y;
  short y0;
  int y0Int;
  short y1;
  int y1Int;
  short y2;
  int y2Int;

  pointsIsShorts = 0;
  if (!((methodArgumentCount()) == 5)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  lineFill = positive32BitValueOf(stackValue(0));
  lineWidth = stackIntegerValue(1);
  fillIndex = positive32BitValueOf(stackValue(2));
  nSegments = stackIntegerValue(3);
  points = stackValue(4);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(5), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }

  /* First, do a check if the points look okay */
  length = slotSizeOf(points);
  if (isWords(points)) {
    pointsIsArray = 0;
    if (length == (nSegments * 3)) {
      pointsIsShorts = 1;
    } else {
      if (length == (nSegments * 6)) {
        pointsIsShorts = 0;
      } else {
        return primitiveFailFor(PrimErrBadArgument);
      }
    }
  } else {
    if (!((isArray(points)) && (length == (nSegments * 3)))) {
      return primitiveFailFor(PrimErrBadArgument);
    }
    pointsIsArray = 1;
  }

  /* Either PointArray or ShortPointArray
     Must be Array of points
     Now check that we have some hope to have enough free space.
     Do this by assuming nPoints boundaries of maximum size,
     hoping that most of the fills will be colors and many boundaries
     will be line segments */
  if ((lineWidth == 0) || (lineFill == 0)) {
    segSize = GLBaseSize;
  } else {
    segSize = GLWideSize;
  }

  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       (segSize * nSegments)) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return primitiveFailFor(GEFWorkTooBig);
  }

  /* Check the fills */
  if (!((isFillOkay(lineFill)) && (isFillOkay(fillIndex)))) {
    return primitiveFailFor(GEFWrongFill);
  }
  lineFill = transformColor(lineFill);
  fillIndex = transformColor(fillIndex);
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }

  /* Check if have anything at all to do */
  if (((lineFill == 0) || (lineWidth == 0)) && (fillIndex == 0)) {
    return pop(5);
  }

  /* Transform the lineWidth */
  if (lineWidth) {
    lineWidth = transformWidth(lineWidth);
    if (lineWidth < 1) {
      lineWidth = 1;
    }
  }

  /* And load the actual shape */
  if (pointsIsArray) {
    loadShapeFromArraynSegmentsfilllineWidthlineFill(
        points, nSegments, fillIndex, lineWidth, lineFill);
  } else {
    if (pointsIsShorts) {
      /* begin loadShapeFromShortPoints:nSegments:fill:lineWidth:lineFill: */
      for (i = 1; i <= nSegments; i += 1) {
        /* begin
         * loadCompressedSegment:fromShortPoints:leftFill:rightFill:lineWidth:lineColor:
         */
        if ((fillIndex == 0) && ((lineWidth == 0) || (lineFill == 0))) {
          continue;
        }

        /* 3 points with x/y each */
        index = (i - 1) * 6;
        x0 = (((short *)(firstIndexableField(points))))[index];
        y0 = (((short *)(firstIndexableField(points))))[index + 1];
        x1 = (((short *)(firstIndexableField(points))))[index + 2];
        y1 = (((short *)(firstIndexableField(points))))[index + 3];
        x2 = (((short *)(firstIndexableField(points))))[index + 4];
        y2 = (((short *)(firstIndexableField(points))))[index + 5];

        /* Briefly check if can represent the bezier as a line */
        if (((x0 == x1) && (y0 == y1)) || ((x1 == x2) && (y1 == y2))) {
          if ((x0 == x2) && (y0 == y2)) {
            continue;
          }
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
              x = ((
                  sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                           ((transform[1]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[2])) *
                         (((double)(workBuffer[GWAALevel])))));
              y = ((
                  sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
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
              x = ((
                  sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                           ((transform[1]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[2])) *
                         (((double)(workBuffer[GWAALevel])))));
              y = ((
                  sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
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
              lineWidth, ((int *)(workBuffer + GWPoint1)),
              ((int *)(workBuffer + GWPoint2)), lineFill, fillIndex, 0);
          if (engineStopped) {
            break;
          }
          continue;
        }

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
            x = ((
                sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                         ((transform[1]) * (((double)((((int *)point))[1]))))) +
                        (transform[2])) *
                       (((double)(workBuffer[GWAALevel])))));
            y = ((
                sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                         ((transform[4]) * (((double)((((int *)point))[1]))))) +
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
            x = ((
                sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                         ((transform[1]) * (((double)((((int *)point))[1]))))) +
                        (transform[2])) *
                       (((double)(workBuffer[GWAALevel])))));
            y = ((
                sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                         ((transform[4]) * (((double)((((int *)point))[1]))))) +
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
            x = ((
                sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                         ((transform[1]) * (((double)((((int *)point))[1]))))) +
                        (transform[2])) *
                       (((double)(workBuffer[GWAALevel])))));
            y = ((
                sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                         ((transform[4]) * (((double)((((int *)point))[1]))))) +
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
            (lineWidth != 0) && (lineFill != 0));
        if (engineStopped) {
          break;
        }
        loadWideBezierlineFillleftFillrightFilln(lineWidth, lineFill, fillIndex,
                                                 0, segs);
        /* end
         * loadCompressedSegment:fromShortPoints:leftFill:rightFill:lineWidth:lineColor:
         */
        if (engineStopped) {
          break;
        }
      }
      /* end loadShapeFromShortPoints:nSegments:fill:lineWidth:lineFill: */
    } else {
      /* begin loadShapeFromIntPoints:nSegments:fill:lineWidth:lineFill: */
      for (i = 1; i <= nSegments; i += 1) {
        /* begin
         * loadCompressedSegment:fromIntPoints:leftFill:rightFill:lineWidth:lineColor:
         */
        if ((fillIndex == 0) && ((lineWidth == 0) || (lineFill == 0))) {
          continue;
        }

        /* 3 points with x/y each */
        index = (i - 1) * 6;
        x0Int = (((int *)(firstIndexableField(points))))[index];
        y0Int = (((int *)(firstIndexableField(points))))[index + 1];
        x1Int = (((int *)(firstIndexableField(points))))[index + 2];
        y1Int = (((int *)(firstIndexableField(points))))[index + 3];
        x2Int = (((int *)(firstIndexableField(points))))[index + 4];
        y2Int = (((int *)(firstIndexableField(points))))[index + 5];

        /* Briefly check if can represent the bezier as a line */
        if (((x0Int == x1Int) && (y0Int == y1Int)) ||
            ((x1Int == x2Int) && (y1Int == y2Int))) {
          if ((x0Int == x2Int) && (y0Int == y2Int)) {
            continue;
          }
          (*((int *)(workBuffer + GWPoint1)) = x0Int);
          (((int *)(workBuffer + GWPoint1)))[1] = y0Int;
          (*((int *)(workBuffer + GWPoint2)) = x2Int);
          (((int *)(workBuffer + GWPoint2)))[1] = y2Int;

          /* begin transformPoints: */
          {
            point = ((int *)(workBuffer + GWPoint1));

            /* begin transformPoint: */
            if (workBuffer[GWHasEdgeTransform]) {
              /* begin transformPoint:into: */
              dstPoint1 = ((int *)point);
              transform = ((float *)(workBuffer + GWEdgeTransform));
              x = ((
                  sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                           ((transform[1]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[2])) *
                         (((double)(workBuffer[GWAALevel])))));
              y = ((
                  sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
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
              x = ((
                  sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                           ((transform[1]) *
                            (((double)((((int *)point))[1]))))) +
                          (transform[2])) *
                         (((double)(workBuffer[GWAALevel])))));
              y = ((
                  sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
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
              lineWidth, ((int *)(workBuffer + GWPoint1)),
              ((int *)(workBuffer + GWPoint2)), lineFill, fillIndex, 0);
          if (engineStopped) {
            break;
          }
          continue;
        }

        /* We can use a line from x0/y0 to x2/y2
           Need bezier curve
           Load and transform points */
        (*((int *)(workBuffer + GWPoint1)) = x0Int);
        (((int *)(workBuffer + GWPoint1)))[1] = y0Int;
        (*((int *)(workBuffer + GWPoint2)) = x1Int);
        (((int *)(workBuffer + GWPoint2)))[1] = y1Int;
        (*((int *)(workBuffer + GWPoint3)) = x2Int);
        (((int *)(workBuffer + GWPoint3)))[1] = y2Int;

        /* begin transformPoints: */
        {
          point = ((int *)(workBuffer + GWPoint1));

          /* begin transformPoint: */
          if (workBuffer[GWHasEdgeTransform]) {
            /* begin transformPoint:into: */
            dstPoint1 = ((int *)point);
            transform = ((float *)(workBuffer + GWEdgeTransform));
            x = ((
                sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                         ((transform[1]) * (((double)((((int *)point))[1]))))) +
                        (transform[2])) *
                       (((double)(workBuffer[GWAALevel])))));
            y = ((
                sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                         ((transform[4]) * (((double)((((int *)point))[1]))))) +
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
            x = ((
                sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                         ((transform[1]) * (((double)((((int *)point))[1]))))) +
                        (transform[2])) *
                       (((double)(workBuffer[GWAALevel])))));
            y = ((
                sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                         ((transform[4]) * (((double)((((int *)point))[1]))))) +
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
            x = ((
                sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                         ((transform[1]) * (((double)((((int *)point))[1]))))) +
                        (transform[2])) *
                       (((double)(workBuffer[GWAALevel])))));
            y = ((
                sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
                         ((transform[4]) * (((double)((((int *)point))[1]))))) +
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
            (lineWidth != 0) && (lineFill != 0));
        if (engineStopped) {
          break;
        }
        loadWideBezierlineFillleftFillrightFilln(lineWidth, lineFill, fillIndex,
                                                 0, segs);
        /* end
         * loadCompressedSegment:fromIntPoints:leftFill:rightFill:lineWidth:lineColor:
         */
        if (engineStopped) {
          break;
        }
      }
      /* end loadShapeFromIntPoints:nSegments:fill:lineWidth:lineFill: */
    }
  }
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }
  if (failed()) {
    return primitiveFailFor(GEFEntityLoadFailed);
  }

  /* needsFlushPut: */
  workBuffer[GWNeedsFlush] = 1;

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(5);
  return 0;
}