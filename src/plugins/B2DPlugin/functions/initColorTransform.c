/* Extracted from B2DPlugin.c:3593 (function initColorTransform). */

static sqInt initColorTransform(void) {
  float *transform;

  /* begin colorTransform */
  transform = ((float *)(workBuffer + GWColorTransform));
  transform[0] = (1.0f);
  transform[1] = (0.0f);
  transform[2] = (1.0f);
  transform[3] = (0.0f);
  transform[4] = (1.0f);
  transform[5] = (0.0f);
  transform[6] = (1.0f);
  transform[7] = (0.0f);

  /* hasColorTransformPut: */
  workBuffer[GWHasColorTransform] = 0;
  return 0;
}