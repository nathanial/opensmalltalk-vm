/* Extracted from BitBltPlugin.c:1309 (function bitOrInvertwith). */

static unsigned int bitOrInvertwith(unsigned int sourceWord,
                                    unsigned int destinationWord) {
  return sourceWord | ((unsigned int)~destinationWord);
}