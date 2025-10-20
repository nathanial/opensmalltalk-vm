/* Extracted from B2DPlugin.c:6802 (function primitiveAddLine). */

EXPORT(sqInt)
primitiveAddLine(void) {
  int *dstPoint1;
  sqInt endOop;
  sqInt failureCode;
  usqInt leftFill;
  int *point;
  usqInt rightFill;
  sqInt startOop;
  float *transform;
  sqInt x;
  sqInt y;

  if (!((methodArgumentCount()) == 4)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  rightFill = positive32BitValueOf(stackValue(0));
  leftFill = positive32BitValueOf(stackValue(1));
  endOop = stackObjectValue(2);
  startOop = stackObjectValue(3);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(4), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }

  /* Make sure the fills are okay */
  if (!((isFillOkay(leftFill)) && (isFillOkay(rightFill)))) {
    return primitiveFailFor(GEFWrongFill);
  }

  /* Load the points */
  loadPointfrom(((int *)(workBuffer + GWPoint1)), startOop);
  loadPointfrom(((int *)(workBuffer + GWPoint2)), endOop);
  if (failed()) {
    return primitiveFailFor(GEFBadPoint);
  }

  /* Transform points */

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

  /* Transform colors */
  leftFill = transformColor(leftFill);
  rightFill = transformColor(rightFill);
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }

  /* Load line */
  loadWideLinefromtolineFillleftFillrightFill(
      0, ((int *)(workBuffer + GWPoint1)), ((int *)(workBuffer + GWPoint2)), 0,
      leftFill, rightFill);
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }
  if (failed()) {
    return primitiveFailFor(GEFEntityLoadFailed);
  }

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(4);
  return 0;
}