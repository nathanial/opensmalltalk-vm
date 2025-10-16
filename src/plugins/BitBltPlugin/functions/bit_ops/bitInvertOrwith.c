/* Extracted from BitBltPlugin.c:1291 (function bitInvertOrwith). */
/* BitBltSimulation>>#bitInvertOr:with: */

static unsigned int bitInvertOrwith(unsigned int sourceWord,
                                    unsigned int destinationWord) {
  return ((unsigned int)~sourceWord) | destinationWord;
}
