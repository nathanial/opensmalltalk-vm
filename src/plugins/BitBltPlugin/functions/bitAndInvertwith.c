/* Extracted from BitBltPlugin.c:1255 (function bitAndInvertwith). */

static unsigned int bitAndInvertwith(unsigned int sourceWord,
                                     unsigned int destinationWord) {
  return sourceWord & ((unsigned int)~destinationWord);
}