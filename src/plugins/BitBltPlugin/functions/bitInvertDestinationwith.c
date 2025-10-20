/* Extracted from BitBltPlugin.c:1279 (function bitInvertDestinationwith). */
/* BitBltSimulation>>#bitInvertDestination:with: */

static unsigned int bitInvertDestinationwith(unsigned int sourceWord,
                                             unsigned int destinationWord) {
  return (unsigned int)~destinationWord;
}
