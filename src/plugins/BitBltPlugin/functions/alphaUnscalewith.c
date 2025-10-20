/* Extracted from BitBltPlugin.c:1211 (function alphaUnscalewith). */
/* BitBltSimulation>>#alphaUnscale:with: */

static unsigned int alphaUnscalewith(unsigned int sourceWord,
                                     unsigned int destinationWord) {
  unsigned int alpha;
  unsigned int b;
  unsigned int carry;
  unsigned int g;
  unsigned int r;
  unsigned int rgb;

  /* Do NOT inline this into optimized loops */
  /* High 8 bits is opacity (ARGB format) */
  alpha = ((usqInt)(destinationWord)) >> 24;
  if (!alpha) {
    return 0;
  }

  /* unscale red components */
  r = (((destinationWord & 0xFF0000) * 0xFF) +
       ((((usqInt)((alpha + 1)) << 15)))) /
      alpha;

  /* unscale green component */
  g = (((destinationWord & 0xFF00) * 0xFF) + ((((usqInt)((alpha + 1)) << 7)))) /
      alpha;

  /* unscale blue components */
  b = (((destinationWord & 0xFF) * 0xFF) + (((usqInt)((alpha + 1))) >> 1)) /
      alpha;
  carry =
      ((usqInt)((((r & 0xFF000000U) | (g & 0xFF0000)) | (b & 0xFF00)))) >> 8;
  carry = carry | ((((usqInt)((carry & 0xAAAAAA))) >> 1) |
                   ((((usqInt)((carry & 0x555555)) << 1))));
  carry = carry | ((((usqInt)((carry & 0xCCCCCC))) >> 2) |
                   ((((usqInt)((carry & 0x333333)) << 2))));
  carry = carry | ((((usqInt)((carry & 0xF0F0F0))) >> 4) |
                   ((((usqInt)((carry & 0xF0F0F)) << 4))));
  rgb = ((r & 0xFF0000) | (g & 0xFF00)) | (b & 0xFF);

  /* saturate RGB components if division overflows */
  rgb = rgb | carry;
  return rgb | (destinationWord & 0xFF000000U);
}
