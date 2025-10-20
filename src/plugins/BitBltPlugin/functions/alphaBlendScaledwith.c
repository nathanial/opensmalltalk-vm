/* Extracted from BitBltPlugin.c:576 (function alphaBlendScaledwith). */

static unsigned int alphaBlendScaledwith(unsigned int sourceWord,
                                         unsigned int destinationWord) {
  unsigned int ag;
  unsigned int rb;
  unsigned int unAlpha;

  /* High 8 bits of source pixel is source opacity (ARGB format) */
  unAlpha = 0xFF - (((usqInt)(sourceWord)) >> 24);

  /* add 16r80 for rounding division to nearest byte */
  rb = ((destinationWord & 0xFF00FF) * unAlpha) + 0x800080;

  /* add 16r80 for rounding division to nearest byte */
  ag = (((((usqInt)(destinationWord)) >> 8) & 0xFF00FF) * unAlpha) + 0x800080;

  /* divide by 255 */
  rb = ((usqInt)((((((usqInt)(rb)) >> 8) & 0xFF00FF) + rb))) >> 8;

  /* divide by 255 */
  ag = ((usqInt)((((((usqInt)(ag)) >> 8) & 0xFF00FF) + ag))) >> 8;

  /* blend red and blue components */
  rb = (rb & 0xFF00FF) + (sourceWord & 0xFF00FF);

  /* blend alpha and green components */
  ag = (ag & 0xFF00FF) + ((((usqInt)(sourceWord)) >> 8) & 0xFF00FF);

  /* saturate red and blue components if there is a carry */
  rb = (rb & 0xFF00FF) | (((usqInt)(((rb & 0x1000100) * 0xFF))) >> 8);

  /* saturate alpha and green components if there is a carry */
  ag = ((((usqInt)((ag & 0xFF00FF)) << 8))) | ((ag & 0x1000100) * 0xFF);
  return ag | rb;
}