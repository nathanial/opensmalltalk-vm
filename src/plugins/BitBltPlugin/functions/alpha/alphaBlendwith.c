/* Extracted from BitBltPlugin.c:684 (function alphaBlendwith). */
/* BitBltSimulation>>#alphaBlend:with: */

static unsigned int alphaBlendwith(unsigned int sourceWord,
                                   unsigned int destinationWord) {
  unsigned int alpha;
  unsigned int blendAG;
  unsigned int blendRB;
  unsigned int result;
  unsigned int unAlpha;

  /* High 8 bits of source pixel */
  alpha = ((usqInt)(sourceWord)) >> 24;
  if (!alpha) {
    return destinationWord;
  }
  if (alpha == 0xFF) {
    return sourceWord;
  }
  unAlpha = 0xFF - alpha;

  /* blend red and blue */
  blendRB = (((sourceWord & 0xFF00FF) * alpha) +
             ((destinationWord & 0xFF00FF) * unAlpha)) +
            0xFF00FF;

  /* blend alpha and green */
  blendAG = (((((((usqInt)(sourceWord)) >> 8) | 0xFF0000) & 0xFF00FF) * alpha) +
             (((((usqInt)(destinationWord)) >> 8) & 0xFF00FF) * unAlpha)) +
            0xFF00FF;

  /* divide by 255 */
  blendRB = (((usqInt)((blendRB + ((((usqInt)((blendRB - 0x10001))) >> 8) &
                                   0xFF00FF)))) >>
             8) &
            0xFF00FF;
  blendAG = (((usqInt)((blendAG + ((((usqInt)((blendAG - 0x10001))) >> 8) &
                                   0xFF00FF)))) >>
             8) &
            0xFF00FF;
  result = blendRB | ((((usqInt)(blendAG) << 8)));
  return result;
}
