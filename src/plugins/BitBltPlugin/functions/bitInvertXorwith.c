/* Extracted from BitBltPlugin.c:1303 (function bitInvertXorwith). */

static unsigned int bitInvertXorwith(unsigned int sourceWord,
                                     unsigned int destinationWord) {
  return ((unsigned int)~sourceWord) ^ destinationWord;
}