/* Extracted from BitBltPlugin.c:626 (function alphaBlendUnscaledwith). */

static unsigned int alphaBlendUnscaledwith(unsigned int sourceWord,
                                           unsigned int destinationWord) {
  unsigned int alpha;
  unsigned int blendA;
  unsigned int blendB;
  unsigned int blendG;
  unsigned int blendR;
  unsigned int result;

  /* High 8 bits of source pixel, assuming ARGB encoding */
  alpha = ((usqInt)(sourceWord)) >> 24;
  if (!alpha) {
    return destinationWord;
  }
  if (alpha == 0xFF) {
    return sourceWord;
  }

  /* blend alpha channels */
  blendA = ((0xFF * alpha) +
            ((0xFF - alpha) * (((usqInt)(destinationWord)) >> 24))) +
           128;

  /* divide by 255 */
  blendA = (((usqInt)(((((usqInt)(blendA)) >> 8) + blendA))) >> 8) & 0xFF;

  /* blend red */
  blendR = (((((sourceWord & 0xFF0000) * alpha) +
              ((destinationWord & 0xFF0000) * (blendA - alpha))) +
             ((((usqInt)(blendA) << 15)))) /
            blendA) &
           0xFF0000;

  /* blend green */
  blendG = (((((sourceWord & 0xFF00) * alpha) +
              ((destinationWord & 0xFF00) * (blendA - alpha))) +
             ((((usqInt)(blendA) << 7)))) /
            blendA) &
           0xFF00;

  /* blend blue */
  blendB = (((((sourceWord & 0xFF) * alpha) +
              ((destinationWord & 0xFF) * (blendA - alpha))) +
             (((usqInt)(blendA)) >> 1)) /
            blendA) &
           0xFF;
  result = ((blendR | blendB) | blendG) | ((((usqInt)(blendA) << 24)));
  return result;
}