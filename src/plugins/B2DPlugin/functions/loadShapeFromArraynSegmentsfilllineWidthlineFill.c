/* Extracted from B2DPlugin.c:5155 (function loadShapeFromArraynSegmentsfilllineWidthlineFill). */

static sqInt loadShapeFromArraynSegmentsfilllineWidthlineFill(sqInt points,
                                                              sqInt nSegments,
                                                              sqInt fillIndex,
                                                              sqInt lineWidth,
                                                              sqInt lineFill) {
  int *dstPoint1;
  sqInt i;
  int *point;
  sqInt pointOop;
  sqInt segs;
  float *transform;
  sqInt x;
  int x0;
  int x1;
  int x2;
  sqInt y;
  int y0;
  int y1;
  int y2;

  for (i = 0; i < nSegments; i += 1) {
    pointOop = fetchPointerofObject(i * 3, points);
    loadPointfrom(((int *)(workBuffer + GWPoint1)), pointOop);
    pointOop = fetchPointerofObject((i * 3) + 1, points);
    loadPointfrom(((int *)(workBuffer + GWPoint2)), pointOop);
    pointOop = fetchPointerofObject((i * 3) + 2, points);
    loadPointfrom(((int *)(workBuffer + GWPoint3)), pointOop);
    if (failed()) {
      return null;
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

    x0 = (*((int *)(workBuffer + GWPoint1)));
    y0 = (((int *)(workBuffer + GWPoint1)))[1];
    x1 = (*((int *)(workBuffer + GWPoint2)));
    y1 = (((int *)(workBuffer + GWPoint2)))[1];
    x2 = (*((int *)(workBuffer + GWPoint3)));
    y2 = (((int *)(workBuffer + GWPoint3)))[1];

    /* Check if we can use a line */
    if (((x0 == y0) && (x1 == y1)) || ((x1 == x2) && (y1 == y2))) {
      loadWideLinefromtolineFillleftFillrightFill(
          lineWidth, ((int *)(workBuffer + GWPoint1)),
          ((int *)(workBuffer + GWPoint3)), lineFill, fillIndex, 0);
    } else {
      segs = loadAndSubdivideBezierFromviatoisWide(
          ((int *)(workBuffer + GWPoint1)), ((int *)(workBuffer + GWPoint2)),
          ((int *)(workBuffer + GWPoint3)),
          (lineWidth != 0) && (lineFill != 0));
      if (engineStopped) {
        return null;
      }
      loadWideBezierlineFillleftFillrightFilln(lineWidth, lineFill, fillIndex,
                                               0, segs);
    }

    /* Need bezier */
    if (engineStopped) {
      return null;
    }
  }
  return 0;
}