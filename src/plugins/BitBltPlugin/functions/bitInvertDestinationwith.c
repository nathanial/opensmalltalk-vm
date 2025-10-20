/* Extracted from BitBltPlugin.c:1279 (function bitInvertDestinationwith). */

static unsigned int bitInvertDestinationwith(unsigned int sourceWord,
                                             unsigned int destinationWord) {
  return (unsigned int)~destinationWord;
}