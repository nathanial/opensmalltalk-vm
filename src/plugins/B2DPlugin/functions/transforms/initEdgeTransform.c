/* Extracted from B2DPlugin.c:3613 (function initEdgeTransform). */

static sqInt initEdgeTransform(void) {
  float *transform;

  /* begin edgeTransform */
  transform = ((float *)(workBuffer + GWEdgeTransform));
  transform[0] = (1.0f);
  transform[1] = (0.0f);
  transform[2] = (0.0f);
  transform[3] = (0.0f);
  transform[4] = (1.0f);
  transform[5] = (0.0f);

  /* hasEdgeTransformPut: */
  workBuffer[GWHasEdgeTransform] = 0;
  return 0;
}