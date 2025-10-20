/* Extracted from B2DPlugin.c:4631 (function loadEdgeTransformFrom). */

static sqInt loadEdgeTransformFrom(sqInt transformOop) {
  sqInt okay;
  float *transform;

  /* hasEdgeTransformPut: */
  workBuffer[GWHasEdgeTransform] = 0;

  /* begin edgeTransform */
  transform = ((float *)(workBuffer + GWEdgeTransform));
  okay = loadTransformFromintolength(transformOop, transform, 6);
  if (failed()) {
    return null;
  }
  if (!okay) {
    return 0;
  }

  /* hasEdgeTransformPut: */
  workBuffer[GWHasEdgeTransform] = 1;

  /* Add the fill offset to the matrix */
  transform[2] =
      (((float)((transform[2]) + (((double)(workBuffer[GWDestOffsetX]))))));
  transform[5] =
      (((float)((transform[5]) + (((double)(workBuffer[GWDestOffsetY]))))));
  return 1;
}