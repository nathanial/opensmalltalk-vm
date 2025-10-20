/* Extracted from BitBltPlugin.c:5160 (function rgbComponentAlpha32with). */
/* BitBltSimulation>>#rgbComponentAlpha32:with: */
/*	This version assumes
        combinationRule = 41
        sourcePixSize = 32
        destPixSize = 8
        sourceForm ~= destForm.
        Note: This is not real blending since we don't have the source colors
        available.  */

static sqInt rgbComponentAlpha32with(sqInt sourceWord, sqInt destinationWord) {
  sqInt a;
  sqInt aA;
  sqInt aB;
  sqInt aG;
  sqInt alpha;
  sqInt answer;
  sqInt aR;
  sqInt b;
  unsigned int d;
  sqInt dstMask;
  sqInt g;
  sqInt r;
  unsigned int s;
  unsigned int srcAlpha;
  sqInt srcColor;

  alpha = sourceWord;
  if (!alpha) {
    return destinationWord;
  }
  srcColor = componentAlphaModeColor;
  srcAlpha = componentAlphaModeAlpha & 0xFF;
  aB = alpha & 0xFF;
  alpha = ((usqInt)(alpha)) >> 8;
  aG = alpha & 0xFF;
  alpha = ((usqInt)(alpha)) >> 8;
  aR = alpha & 0xFF;
  alpha = ((usqInt)(alpha)) >> 8;
  aA = alpha & 0xFF;
  if (!(srcAlpha == 0xFF)) {
    aA = ((usqInt)((aA * srcAlpha))) >> 8;
    aR = ((usqInt)((aR * srcAlpha))) >> 8;
    aG = ((usqInt)((aG * srcAlpha))) >> 8;
    aB = ((usqInt)((aB * srcAlpha))) >> 8;
  }
  dstMask = destinationWord;
  d = dstMask & 0xFF;
  s = srcColor & 0xFF;
  if (ungammaLookupTable) {
    d = ungammaLookupTable[d];
    s = ungammaLookupTable[s];
  }
  b = (((usqInt)((d * (0xFF - aB)))) >> 8) + (((usqInt)((s * aB))) >> 8);
  if (b > 0xFF) {
    b = 0xFF;
  }
  if (gammaLookupTable) {
    b = gammaLookupTable[b];
  }
  dstMask = ((usqInt)(dstMask)) >> 8;
  srcColor = ((usqInt)(srcColor)) >> 8;
  d = dstMask & 0xFF;
  s = srcColor & 0xFF;
  if (ungammaLookupTable) {
    d = ungammaLookupTable[d];
    s = ungammaLookupTable[s];
  }
  g = (((usqInt)((d * (0xFF - aG)))) >> 8) + (((usqInt)((s * aG))) >> 8);
  if (g > 0xFF) {
    g = 0xFF;
  }
  if (gammaLookupTable) {
    g = gammaLookupTable[g];
  }
  dstMask = ((usqInt)(dstMask)) >> 8;
  srcColor = ((usqInt)(srcColor)) >> 8;
  d = dstMask & 0xFF;
  s = srcColor & 0xFF;
  if (ungammaLookupTable) {
    d = ungammaLookupTable[d];
    s = ungammaLookupTable[s];
  }
  r = (((usqInt)((d * (0xFF - aR)))) >> 8) + (((usqInt)((s * aR))) >> 8);
  if (r > 0xFF) {
    r = 0xFF;
  }
  if (gammaLookupTable) {
    r = gammaLookupTable[r];
  }
  dstMask = ((usqInt)(dstMask)) >> 8;
  srcColor = ((usqInt)(srcColor)) >> 8;

  /* no need to gamma correct alpha value ? */
  a = (((usqInt)(((dstMask & 0xFF) * (0xFF - aA)))) >> 8) + aA;
  if (a > 0xFF) {
    a = 0xFF;
  }
  answer = ((((usqInt)((((((usqInt)((((((usqInt)(a) << 8))) + r)) << 8))) + g))
              << 8))) +
           b;
  return answer;
}
