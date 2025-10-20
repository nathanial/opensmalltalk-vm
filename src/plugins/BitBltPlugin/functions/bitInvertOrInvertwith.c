/* Extracted from BitBltPlugin.c:1285 (function bitInvertOrInvertwith). */
/* BitBltSimulation>>#bitInvertOrInvert:with: */

static unsigned int bitInvertOrInvertwith(unsigned int sourceWord,
                                          unsigned int destinationWord) {
  return ((unsigned int)~sourceWord) | ((unsigned int)~destinationWord);
}
