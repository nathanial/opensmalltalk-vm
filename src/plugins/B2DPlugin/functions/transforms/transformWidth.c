/* Extracted from B2DPlugin.c:10917 (function transformWidth). */

static sqInt transformWidth(sqInt w) {
  double deltaX;
  double deltaY;
  int *dstPoint1;
  sqInt dstWidth;
  sqInt dstWidth2;
  int *point;
  float *transform;
  sqInt x;
  sqInt y;

  if (!w) {
    return 0;
  }
  (*((int *)(workBuffer + GWPoint1)) = 0);
  (((int *)(workBuffer + GWPoint1)))[1] = 0;
  (*((int *)(workBuffer + GWPoint2)) = w * 0x100);
  (((int *)(workBuffer + GWPoint2)))[1] = 0;
  (*((int *)(workBuffer + GWPoint3)) = 0);
  (((int *)(workBuffer + GWPoint3)))[1] = (w * 0x100);

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

  deltaX = ((double)(((*((int *)(workBuffer + GWPoint2)))) -
                     ((*((int *)(workBuffer + GWPoint1))))));
  deltaY = ((double)(((((int *)(workBuffer + GWPoint2)))[1]) -
                     ((((int *)(workBuffer + GWPoint1)))[1])));
  dstWidth =
      ((((sqInt)(sqrt((deltaX * deltaX) + (deltaY * deltaY))))) + 128) / 0x100;
  deltaX = ((double)(((*((int *)(workBuffer + GWPoint3)))) -
                     ((*((int *)(workBuffer + GWPoint1))))));
  deltaY = ((double)(((((int *)(workBuffer + GWPoint3)))[1]) -
                     ((((int *)(workBuffer + GWPoint1)))[1])));
  dstWidth2 =
      ((((sqInt)(sqrt((deltaX * deltaX) + (deltaY * deltaY))))) + 128) / 0x100;
  if (dstWidth2 < dstWidth) {
    dstWidth = dstWidth2;
  }
  if (dstWidth) {
    return dstWidth;
  } else {
    return 1;
  }
}