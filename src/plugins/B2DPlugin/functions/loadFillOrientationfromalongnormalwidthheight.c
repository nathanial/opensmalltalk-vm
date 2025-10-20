/* Extracted from B2DPlugin.c:4662 (function loadFillOrientationfromalongnormalwidthheight). */

static sqInt loadFillOrientationfromalongnormalwidthheight(
    sqInt fill, int *point1, int *point2, int *point3, sqInt fillWidth,
    sqInt fillHeight) {
  int dirX;
  int dirY;
  int dsLength2;
  int *dstPoint1;
  sqInt dsX;
  sqInt dsY;
  int dtLength2;
  sqInt dtX;
  sqInt dtY;
  int nrmX;
  int nrmY;
  float *transform;
  sqInt x;
  sqInt y;

  point2[0] = ((point2[0]) + (point1[0]));
  point2[1] = ((point2[1]) + (point1[1]));
  point3[0] = ((point3[0]) + (point1[0]));
  point3[1] = ((point3[1]) + (point1[1]));

  /* begin transformPoint: */
  if (workBuffer[GWHasEdgeTransform]) {
    /* begin transformPoint:into: */
    dstPoint1 = ((int *)point1);
    transform = ((float *)(workBuffer + GWEdgeTransform));
    x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point1)))))) +
                   ((transform[1]) * (((double)((((int *)point1))[1]))))) +
                  (transform[2])) *
                 (((double)(workBuffer[GWAALevel])))));
    y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point1)))))) +
                   ((transform[4]) * (((double)((((int *)point1))[1]))))) +
                  (transform[5])) *
                 (((double)(workBuffer[GWAALevel])))));
    dstPoint1[0] = x;
    dstPoint1[1] = y;
  } else {
    point1[0] =
        (((point1[0]) + (workBuffer[GWDestOffsetX])) * (workBuffer[GWAALevel]));
    point1[1] =
        (((point1[1]) + (workBuffer[GWDestOffsetY])) * (workBuffer[GWAALevel]));
  }

  /* begin transformPoint: */
  if (workBuffer[GWHasEdgeTransform]) {
    /* begin transformPoint:into: */
    dstPoint1 = ((int *)point2);
    transform = ((float *)(workBuffer + GWEdgeTransform));
    x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point2)))))) +
                   ((transform[1]) * (((double)((((int *)point2))[1]))))) +
                  (transform[2])) *
                 (((double)(workBuffer[GWAALevel])))));
    y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point2)))))) +
                   ((transform[4]) * (((double)((((int *)point2))[1]))))) +
                  (transform[5])) *
                 (((double)(workBuffer[GWAALevel])))));
    dstPoint1[0] = x;
    dstPoint1[1] = y;
  } else {
    point2[0] =
        (((point2[0]) + (workBuffer[GWDestOffsetX])) * (workBuffer[GWAALevel]));
    point2[1] =
        (((point2[1]) + (workBuffer[GWDestOffsetY])) * (workBuffer[GWAALevel]));
  }

  /* begin transformPoint: */
  if (workBuffer[GWHasEdgeTransform]) {
    /* begin transformPoint:into: */
    dstPoint1 = ((int *)point3);
    transform = ((float *)(workBuffer + GWEdgeTransform));
    x = ((sqInt)(((((transform[0]) * (((double)((*((int *)point3)))))) +
                   ((transform[1]) * (((double)((((int *)point3))[1]))))) +
                  (transform[2])) *
                 (((double)(workBuffer[GWAALevel])))));
    y = ((sqInt)(((((transform[3]) * (((double)((*((int *)point3)))))) +
                   ((transform[4]) * (((double)((((int *)point3))[1]))))) +
                  (transform[5])) *
                 (((double)(workBuffer[GWAALevel])))));
    dstPoint1[0] = x;
    dstPoint1[1] = y;
  } else {
    point3[0] =
        (((point3[0]) + (workBuffer[GWDestOffsetX])) * (workBuffer[GWAALevel]));
    point3[1] =
        (((point3[1]) + (workBuffer[GWDestOffsetY])) * (workBuffer[GWAALevel]));
  }
  dirX = (point2[0]) - (point1[0]);
  dirY = (point2[1]) - (point1[1]);
  nrmX = (point3[0]) - (point1[0]);
  nrmY = (point3[1]) - (point1[1]);

  /* Compute the scale from direction/normal into ramp size */
  dsLength2 = (dirX * dirX) + (dirY * dirY);
  if (dsLength2 > 0) {
    dsX = ((sqInt)((((((double)dirX)) * (((double)fillWidth))) * 65536.0) /
                   (((double)dsLength2))));
    dsY = ((sqInt)((((((double)dirY)) * (((double)fillWidth))) * 65536.0) /
                   (((double)dsLength2))));
  } else {
    dsX = 0;
    dsY = 0;
  }
  dtLength2 = (nrmX * nrmX) + (nrmY * nrmY);
  if (dtLength2 > 0) {
    dtX = ((sqInt)((((((double)nrmX)) * (((double)fillHeight))) * 65536.0) /
                   (((double)dtLength2))));
    dtY = ((sqInt)((((((double)nrmY)) * (((double)fillHeight))) * 65536.0) /
                   (((double)dtLength2))));
  } else {
    dtX = 0;
    dtY = 0;
  }

  /* #fillOriginXOf:put: #obj:at:put: */
  objBuffer[fill + GFOriginX] = (point1[0]);

  /* #fillOriginYOf:put: #obj:at:put: */
  objBuffer[fill + GFOriginY] = (point1[1]);

  /* #fillDirectionXOf:put: #obj:at:put: */
  objBuffer[fill + GFDirectionX] = dsX;

  /* #fillDirectionYOf:put: #obj:at:put: */
  objBuffer[fill + GFDirectionY] = dsY;

  /* #fillNormalXOf:put: #obj:at:put: */
  objBuffer[fill + GFNormalX] = dtX;

  /* #fillNormalYOf:put: #obj:at:put: */
  objBuffer[fill + GFNormalY] = dtY;
  return 0;
}