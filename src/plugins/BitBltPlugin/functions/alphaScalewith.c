/* Extracted from BitBltPlugin.c:738 (function alphaScalewith). */

static unsigned int alphaScalewith(unsigned int sourceWord,
                                   unsigned int destinationWord) {
  unsigned int alpha;
  unsigned int g;
  unsigned int rb;

  /* Do NOT inline this into optimized loops */
  /* High 8 bits is opacity (ARGB format) */
  alpha = ((usqInt)(destinationWord)) >> 24;

  /* scale red and blue components */
  rb = ((destinationWord & 0xFF00FF) * alpha) + 0x800080;

  /* divide by 255 */
  rb =
      (((usqInt)((rb + ((((usqInt)((rb - 0x10001))) >> 8) & 0xFF00FF)))) >> 8) &
      0xFF00FF;

  /* scale green component */
  g = ((destinationWord & 0xFF00) * alpha) + 0x8000;

  /* divide by 255 */
  g = (((usqInt)((g + ((((usqInt)((g - 0x100))) >> 8) & 0xFF00)))) >> 8) &
      0xFF00;
  return (g | rb) | (destinationWord & 0xFF000000U);
}