/* Extracted from BitBltPlugin.c:473 (function alphaBlendConstwithpaintMode). */

static unsigned int alphaBlendConstwithpaintMode(unsigned int sourceWord,
                                                 unsigned int destinationWord,
                                                 sqInt paintMode) {
  sqInt bitsPerColor;
  unsigned int blend;
  unsigned int blendAG;
  unsigned int blendRB;
  unsigned int destPixVal;
  unsigned int destShifted;
  sqInt i;
  sqInt j;
  unsigned int maskShifted;
  unsigned int pixBlend;
  unsigned int pixMask;
  unsigned int result;
  unsigned int rgbMask;
  sqInt shift;
  unsigned int sourcePixVal;
  unsigned int sourceShifted;
  unsigned int unAlpha;

  if (destDepth < 16) {
    return destinationWord;
  }
  unAlpha = 0xFF - sourceAlpha;
  result = destinationWord;
  if (destPPW == 1) {
    if (!(paintMode && (sourceWord == 0))) {
      /* blend red and blue */
      blendRB = (((sourceWord & 0xFF00FF) * sourceAlpha) +
                 ((destinationWord & 0xFF00FF) * unAlpha)) +
                0x800080;

      /* blend alpha and green */
      blendAG = ((((((usqInt)(sourceWord)) >> 8) & 0xFF00FF) * sourceAlpha) +
                 (((((usqInt)(destinationWord)) >> 8) & 0xFF00FF) * unAlpha)) +
                0x800080;

      /* divide by 255 */
      blendRB =
          (((usqInt)((((((usqInt)(blendRB)) >> 8) & 0xFF00FF) + blendRB))) >>
           8) &
          0xFF00FF;
      blendAG =
          (((usqInt)((((((usqInt)(blendAG)) >> 8) & 0xFF00FF) + blendAG))) >>
           8) &
          0xFF00FF;
      result = blendRB | ((((usqInt)(blendAG) << 8)));
    }
  } else {
    pixMask = maskTable[destDepth];
    bitsPerColor = 5;
    rgbMask = 0x1F;
    maskShifted = destMask;
    destShifted = destinationWord;
    sourceShifted = sourceWord;
    for (j = 1; j <= destPPW; j += 1) {
      sourcePixVal = sourceShifted & pixMask;
      if (!(((maskShifted & pixMask) == 0) ||
            (paintMode && (sourcePixVal == 0)))) {
        destPixVal = destShifted & pixMask;
        pixBlend = 0;
        for (i = 1; i <= 3; i += 1) {
          shift = (i - 1) * bitsPerColor;

          /* +128 for rounding */
          blend =
              ((((((usqInt)(sourcePixVal)) >> shift) & rgbMask) * sourceAlpha) +
               (((((usqInt)(destPixVal)) >> shift) & rgbMask) * unAlpha)) +
              128;

          /* divide by 255 */
          blend =
              (((usqInt)(((((usqInt)(blend)) >> 8) + blend))) >> 8) & rgbMask;
          pixBlend = pixBlend | ((((usqInt)(blend) << shift)));
        }
        result = (result &
                  ((unsigned int)~((((usqInt)(pixMask) << ((j - 1) * 16)))))) |
                 ((((usqInt)(pixBlend) << ((j - 1) * 16))));
      }

      /* no effect if outside of dest rectangle */
      maskShifted = ((usqInt)(maskShifted)) >> destDepth;
      sourceShifted = ((usqInt)(sourceShifted)) >> destDepth;
      destShifted = ((usqInt)(destShifted)) >> destDepth;
    }
  }

  /* 32bpp blends include alpha */
  return result;
}