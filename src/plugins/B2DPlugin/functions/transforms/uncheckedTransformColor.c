/* Extracted from B2DPlugin.c:11038 (function uncheckedTransformColor). */

static sqInt uncheckedTransformColor(sqInt fillIndex) {
  sqInt a;
  sqInt b;
  sqInt g;
  sqInt r;
  float *transform;

  if (!(workBuffer[GWHasColorTransform])) {
    return fillIndex;
  }
  b = fillIndex & 0xFF;
  g = (((usqInt)(fillIndex)) >> 8) & 0xFF;
  r = (((usqInt)(fillIndex)) >> 16) & 0xFF;
  a = (((usqInt)(fillIndex)) >> 24) & 0xFF;

  /* begin colorTransform */
  transform = ((float *)(workBuffer + GWColorTransform));
  r = ((sqInt)((r * (transform[0])) + (transform[1])));
  g = ((sqInt)((g * (transform[2])) + (transform[3])));
  b = ((sqInt)((b * (transform[4])) + (transform[5])));
  a = ((sqInt)((a * (transform[6])) + (transform[7])));
  r = ((r < 0) ? 0 : r);
  r = ((r < 0xFF) ? r : 0xFF);
  g = ((g < 0) ? 0 : g);
  g = ((g < 0xFF) ? g : 0xFF);
  b = ((b < 0) ? 0 : b);
  b = ((b < 0xFF) ? b : 0xFF);
  a = ((a < 0) ? 0 : a);
  a = ((a < 0xFF) ? a : 0xFF);
  if (a < 16) {
    return 0;
  }
  return ((b + ((((usqInt)(g) << 8)))) + ((((usqInt)(r) << 16)))) +
         ((((usqInt)(a) << 24)));
}