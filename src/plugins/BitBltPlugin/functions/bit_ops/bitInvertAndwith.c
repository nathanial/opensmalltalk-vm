/* Extracted from BitBltPlugin.c:1273 (function bitInvertAndwith). */
/* BitBltSimulation>>#bitInvertAnd:with: */

static unsigned int bitInvertAndwith(unsigned int sourceWord,
                                     unsigned int destinationWord) {
  return ((unsigned int)~sourceWord) & destinationWord;
}
