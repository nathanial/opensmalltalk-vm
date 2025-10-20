/* Extracted from BitBltPlugin.c:576 (function alphaBlendScaledwith). */
/* BitBltSimulation>>#alphaBlendScaled:with: */
/*	Blend sourceWord with destinationWord using the alpha value from both
        sourceWord and destinationWord.
        Alpha is encoded as 0 meaning 0.0, and 255 meaning 1.0.
        The alpha channel and color produced are

        srcAlpha + (destAlpha*(1-srcAlpha))
        (srcAlpha*srcColor + (destAlpha*(1-srcAlpha)*dstColor)) / (srcAlpha +
        (destAlpha*(1-srcAlpha)))
        In contrast to alphaBlend:with: the method does not assume that
        destination form is opaque.
        In contrast to alphaBlendScaled:with: the method does not assume that
        colors have been pre-scaled (muliplied) by alpha channel.
 */

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
