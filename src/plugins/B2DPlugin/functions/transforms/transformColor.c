/* Extracted from B2DPlugin.c:10869 (function transformColor). */

static sqInt transformColor(sqInt fillIndex) {
  sqInt a;
  double alphaScale;
  sqInt b;
  sqInt g;
  sqInt r;
  float *transform;

  if (!((fillIndex == 0) || ((fillIndex & 0xFF000000U) != 0))) {
    return fillIndex;
  }
  b = fillIndex & 0xFF;
  g = (((usqInt)(fillIndex)) >> 8) & 0xFF;
  r = (((usqInt)(fillIndex)) >> 16) & 0xFF;
  a = (((usqInt)(fillIndex)) >> 24) & 0xFF;
  if (workBuffer[GWHasColorTransform]) {
    /* begin colorTransform */
    transform = ((float *)(workBuffer + GWColorTransform));
    alphaScale = ((a * (transform[6])) + (transform[7])) / a;
    r = ((sqInt)(((r * (transform[0])) + (transform[1])) * alphaScale));
    g = ((sqInt)(((g * (transform[2])) + (transform[3])) * alphaScale));
    b = ((sqInt)(((b * (transform[4])) + (transform[5])) * alphaScale));
    a = ((sqInt)(a * alphaScale));
    r = ((r < 0) ? 0 : r);
    r = ((r < 0xFF) ? r : 0xFF);
    g = ((g < 0) ? 0 : g);
    g = ((g < 0xFF) ? g : 0xFF);
    b = ((b < 0) ? 0 : b);
    b = ((b < 0xFF) ? b : 0xFF);
    a = ((a < 0) ? 0 : a);
    a = ((a < 0xFF) ? a : 0xFF);
  }
  if (a < 1) {
    return 0;
  }
  if ((a < 0xFF) && ((workBuffer[GWNeedsFlush]) != 0)) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNeedFlush;
    engineStopped = 1;
  }
  return ((b + ((((usqInt)(g) << 8)))) + ((((usqInt)(r) << 16)))) +
         ((((usqInt)(a) << 24)));
}