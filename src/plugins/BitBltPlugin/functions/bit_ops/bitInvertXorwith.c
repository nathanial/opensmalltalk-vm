/* Extracted from BitBltPlugin.c:1303 (function bitInvertXorwith). */
/* BitBltSimulation>>#bitInvertXor:with: */

static unsigned int bitInvertXorwith(unsigned int sourceWord,
                                     unsigned int destinationWord) {
  return ((unsigned int)~sourceWord) ^ destinationWord;
}
