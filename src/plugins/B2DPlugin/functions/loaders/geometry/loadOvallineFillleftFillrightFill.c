/* Extracted from B2DPlugin.c:4949 (function loadOvallineFillleftFillrightFill). */

static sqInt loadOvallineFillleftFillrightFill(sqInt lineWidth, sqInt lineFill,
                                               sqInt leftFill,
                                               sqInt rightFill) {
  int cx;
  int cy;
  int *dstPoint1;
  int h;
  sqInt i;
  sqInt nSegments;
  int *point;
  float *transform;
  int w;
  sqInt x;
  sqInt y;

  w = (((*((int *)(workBuffer + GWPoint2)))) -
       ((*((int *)(workBuffer + GWPoint1))))) /
      2;
  h = (((((int *)(workBuffer + GWPoint2)))[1]) -
       ((((int *)(workBuffer + GWPoint1)))[1])) /
      2;
  cx = (((*((int *)(workBuffer + GWPoint2)))) +
        ((*((int *)(workBuffer + GWPoint1))))) /
       2;
  cy = (((((int *)(workBuffer + GWPoint2)))[1]) +
        ((((int *)(workBuffer + GWPoint1)))[1])) /
       2;
  for (i = 0; i <= 15; i += 1) {
    loadOvalSegmentwhcxcy(i, w, h, cx, cy);

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
        ((int *)(workBuffer + GWPoint3)), (lineWidth != 0) && (lineFill != 0));
    if (engineStopped) {
      return null;
    }
    loadWideBezierlineFillleftFillrightFilln(lineWidth, lineFill, leftFill,
                                             rightFill, nSegments);
    if (engineStopped) {
      return null;
    }
  }
  return 0;
}