/* Extracted from BitBltPlugin.c:1267 (function bitInvertAndInvertwith). */

static unsigned int bitInvertAndInvertwith(unsigned int sourceWord,
                                           unsigned int destinationWord) {
  return ((unsigned int)~sourceWord) & ((unsigned int)~destinationWord);
}