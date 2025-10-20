/* Extracted from B2DPlugin.c:7016 (function primitiveAddPolygon). */

EXPORT(sqInt)
primitiveAddPolygon(void) {
  int *dstPoint1;
  sqInt failureCode;
  usqInt fillIndex;
  sqInt i;
  sqInt length;
  usqInt lineFill;
  sqInt lineWidth;
  sqInt nPoints;
  int *point;
  sqInt points;
  sqInt pointsIsArray;
  sqInt pointsIsShorts;
  sqInt segSize;
  float *transform;
  sqInt x;
  int x0;
  short x0Short;
  int x1;
  short x1Short;
  sqInt y;
  int y0;
  short y0Short;
  int y1;
  short y1Short;

  pointsIsShorts = 0;
  if (!((methodArgumentCount()) == 5)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  lineFill = positive32BitValueOf(stackValue(0));
  lineWidth = stackIntegerValue(1);
  fillIndex = positive32BitValueOf(stackValue(2));
  nPoints = stackIntegerValue(3);
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
    if (length == nPoints) {
      pointsIsShorts = 1;
    } else {
      if ((nPoints * 2) == length) {
        pointsIsShorts = 0;
      } else {
        return primitiveFailFor(PrimErrBadArgument);
      }
    }
  } else {
    if (!((isArray(points)) && (length == nPoints))) {
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
       (segSize * nPoints)) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return primitiveFail();
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
  }

  /* And load the actual polygon */
  if (pointsIsArray) {
    /* begin loadPolygonFromArray:nPoints:fill:lineWidth:lineFill: */
    loadPointfrom(((int *)(workBuffer + GWPoint1)),
                  fetchPointerofObject(0, points));
    if (failed()) {
      return primitiveFailFor(GEFBadPoint);
    }
    x0 = (*((int *)(workBuffer + GWPoint1)));
    y0 = (((int *)(workBuffer + GWPoint1)))[1];
    for (i = 1; i < nPoints; i += 1) {
      loadPointfrom(((int *)(workBuffer + GWPoint1)),
                    fetchPointerofObject(i, points));
      if (failed()) {
        return primitiveFailFor(GEFBadPoint);
      }
      x1 = (*((int *)(workBuffer + GWPoint1)));
      y1 = (((int *)(workBuffer + GWPoint1)))[1];
      (*((int *)(workBuffer + GWPoint1)) = x0);
      (((int *)(workBuffer + GWPoint1)))[1] = y0;
      (*((int *)(workBuffer + GWPoint2)) = x1);
      (((int *)(workBuffer + GWPoint2)))[1] = y1;

      /* begin transformPoints: */
      {
        point = ((int *)(workBuffer + GWPoint1));

        /* begin transformPoint: */
        if (workBuffer[GWHasEdgeTransform]) {
          /* begin transformPoint:into: */
          dstPoint1 = ((int *)point);
          transform = ((float *)(workBuffer + GWEdgeTransform));
          x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                         ((transform[1]) * (((double)((((int *)point))[1]))))) +
                        (transform[2])) *
                       (((double)(workBuffer[GWAALevel])))));
          y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
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
          x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point)))))) +
                         ((transform[1]) * (((double)((((int *)point))[1]))))) +
                        (transform[2])) *
                       (((double)(workBuffer[GWAALevel])))));
          y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point)))))) +
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

      loadWideLinefromtolineFillleftFillrightFill(
          lineWidth, ((int *)(workBuffer + GWPoint1)),
          ((int *)(workBuffer + GWPoint2)), lineFill, fillIndex, 0);
      if (engineStopped) {
        break;
      }
      x0 = x1;
      y0 = y1;
    }
    /* end loadPolygonFromArray:nPoints:fill:lineWidth:lineFill: */
  } else {
    if (pointsIsShorts) {
      /* begin loadPolygonFromShortPoints:nPoints:fill:lineWidth:lineFill: */
      x0Short = (*((short *)(firstIndexableField(points))));
      y0Short = (((short *)(firstIndexableField(points))))[1];
      for (i = 1; i < nPoints; i += 1) {
        x1Short = (((short *)(firstIndexableField(points))))[i * 2];
        y1Short = (((short *)(firstIndexableField(points))))[(i * 2) + 1];
        (*((int *)(workBuffer + GWPoint1)) = x0Short);
        (((int *)(workBuffer + GWPoint1)))[1] = y0Short;
        (*((int *)(workBuffer + GWPoint2)) = x1Short);
        (((int *)(workBuffer + GWPoint2)))[1] = y1Short;

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

        loadWideLinefromtolineFillleftFillrightFill(
            lineWidth, ((int *)(workBuffer + GWPoint1)),
            ((int *)(workBuffer + GWPoint2)), lineFill, fillIndex, 0);
        if (engineStopped) {
          break;
        }
        x0Short = x1Short;
        y0Short = y1Short;
      }
      /* end loadPolygonFromShortPoints:nPoints:fill:lineWidth:lineFill: */
    } else {
      /* begin loadPolygonFromIntPoints:nPoints:fill:lineWidth:lineFill: */
      x0 = (*((int *)(firstIndexableField(points))));
      y0 = (((int *)(firstIndexableField(points))))[1];
      for (i = 1; i < nPoints; i += 1) {
        x1 = (((int *)(firstIndexableField(points))))[i * 2];
        y1 = (((int *)(firstIndexableField(points))))[(i * 2) + 1];
        (*((int *)(workBuffer + GWPoint1)) = x0);
        (((int *)(workBuffer + GWPoint1)))[1] = y0;
        (*((int *)(workBuffer + GWPoint2)) = x1);
        (((int *)(workBuffer + GWPoint2)))[1] = y1;

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

        loadWideLinefromtolineFillleftFillrightFill(
            lineWidth, ((int *)(workBuffer + GWPoint1)),
            ((int *)(workBuffer + GWPoint2)), lineFill, fillIndex, 0);
        if (engineStopped) {
          break;
        }
        x0 = x1;
        y0 = y1;
      }
      /* end loadPolygonFromIntPoints:nPoints:fill:lineWidth:lineFill: */
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