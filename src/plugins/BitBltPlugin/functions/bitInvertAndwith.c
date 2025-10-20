/* Extracted from BitBltPlugin.c:1273 (function bitInvertAndwith). */

static unsigned int bitInvertAndwith(unsigned int sourceWord,
                                     unsigned int destinationWord) {
  return ((unsigned int)~sourceWord) & destinationWord;
}