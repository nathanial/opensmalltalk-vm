/* Extracted from B2DPlugin.c:5742 (function primitiveAddBezier). */

EXPORT(sqInt)
primitiveAddBezier(void) {
  int *dstPoint1;
  sqInt endOop;
  sqInt failureCode;
  usqInt leftFill;
  sqInt nSegments;
  int *point;
  usqInt rightFill;
  sqInt startOop;
  float *transform;
  sqInt viaOop;
  sqInt x;
  sqInt y;

  if (!((methodArgumentCount()) == 5)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  rightFill = positive32BitValueOf(stackValue(0));
  leftFill = positive32BitValueOf(stackValue(1));
  viaOop = stackObjectValue(2);
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
  if (!((isFillOkay(leftFill)) && (isFillOkay(rightFill)))) {
    return primitiveFailFor(GEFWrongFill);
  }

  /* Do a quick check if the fillIndices are equal - if so, just ignore it */
  if ((leftFill == rightFill) && 0) {
    return pop(6);
  }
  loadPointfrom(((int *)(workBuffer + GWPoint1)), startOop);
  loadPointfrom(((int *)(workBuffer + GWPoint2)), viaOop);
  loadPointfrom(((int *)(workBuffer + GWPoint3)), endOop);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }

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

  nSegments = loadAndSubdivideBezierFromviatoisWide(
      ((int *)(workBuffer + GWPoint1)), ((int *)(workBuffer + GWPoint2)),
      ((int *)(workBuffer + GWPoint3)), 0);

  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       (nSegments * GBBaseSize)) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
  }
  /* end needAvailableSpace: */

  if (!engineStopped) {
    leftFill = transformColor(leftFill);
    rightFill = transformColor(rightFill);
  }
  if (!engineStopped) {
    loadWideBezierlineFillleftFillrightFilln(0, 0, leftFill, rightFill,
                                             nSegments);
  }
  if (engineStopped) {
    /* begin wbStackClear */
    /* wbTopPut: */
    workBuffer[GWBufferTop] = (workBuffer[GWSize]);
    return primitiveFailFor(GEFEngineStopped);
  }

  /* Make sure the stack is okay */
  if (failed()) {
    return primitiveFailFor(GEFEntityLoadFailed);
  }

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(5);
  return 0;
}