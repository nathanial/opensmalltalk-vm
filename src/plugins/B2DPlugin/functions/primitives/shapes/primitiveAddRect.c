/* Extracted from B2DPlugin.c:7387 (function primitiveAddRect). */

EXPORT(sqInt)
primitiveAddRect(void) {
  usqInt borderIndex;
  sqInt borderWidth;
  int *dstPoint1;
  sqInt endOop;
  sqInt failureCode;
  usqInt fillIndex;
  int *point;
  sqInt startOop;
  float *transform;
  sqInt x;
  sqInt y;

  if (!((methodArgumentCount()) == 5)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  borderIndex = positive32BitValueOf(stackValue(0));
  borderWidth = stackIntegerValue(1);
  fillIndex = positive32BitValueOf(stackValue(2));
  endOop = stackObjectValue(3);
  startOop = stackObjectValue(4);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(5), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }

  /* Make sure the fills are okay */
  if (!((isFillOkay(borderIndex)) && (isFillOkay(fillIndex)))) {
    return primitiveFailFor(GEFWrongFill);
  }

  /* Transform colors */
  borderIndex = transformColor(borderIndex);
  fillIndex = transformColor(fillIndex);
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }

  /* Check if we have anything at all to do */
  if ((fillIndex == 0) && ((borderIndex == 0) || (borderWidth == 0))) {
    return pop(5);
  }

  /* Make sure we have some space */

  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       (4 * GLBaseSize)) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return primitiveFailFor(GEFWorkTooBig);
  }

  /* Check if we need a border */
  if ((borderWidth > 0) && (borderIndex != 0)) {
    borderWidth = transformWidth(borderWidth);
  } else {
    borderWidth = 0;
  }

  /* Load the rectangle */
  loadPointfrom(((int *)(workBuffer + GWPoint1)), startOop);
  loadPointfrom(((int *)(workBuffer + GWPoint3)), endOop);
  if (failed()) {
    return primitiveFailFor(GEFBadPoint);
  }
  (*((int *)(workBuffer + GWPoint2)) = (*((int *)(workBuffer + GWPoint3))));
  (((int *)(workBuffer + GWPoint2)))[1] =
      ((((int *)(workBuffer + GWPoint1)))[1]);
  (*((int *)(workBuffer + GWPoint4)) = (*((int *)(workBuffer + GWPoint1))));
  (((int *)(workBuffer + GWPoint4)))[1] =
      ((((int *)(workBuffer + GWPoint3)))[1]);

  /* Transform the points */

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
  {
    point = ((int *)(workBuffer + GWPoint3));

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
    point = ((int *)(workBuffer + GWPoint4));

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

  /* begin loadRectangle:lineFill:leftFill:rightFill: */
  loadWideLinefromtolineFillleftFillrightFill(
      borderWidth, ((int *)(workBuffer + GWPoint1)),
      ((int *)(workBuffer + GWPoint2)), borderIndex, 0, fillIndex);
  loadWideLinefromtolineFillleftFillrightFill(
      borderWidth, ((int *)(workBuffer + GWPoint2)),
      ((int *)(workBuffer + GWPoint3)), borderIndex, 0, fillIndex);
  loadWideLinefromtolineFillleftFillrightFill(
      borderWidth, ((int *)(workBuffer + GWPoint3)),
      ((int *)(workBuffer + GWPoint4)), borderIndex, 0, fillIndex);
  loadWideLinefromtolineFillleftFillrightFill(
      borderWidth, ((int *)(workBuffer + GWPoint4)),
      ((int *)(workBuffer + GWPoint1)), borderIndex, 0, fillIndex);
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