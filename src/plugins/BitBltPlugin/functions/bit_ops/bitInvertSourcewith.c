/* Extracted from BitBltPlugin.c:1297 (function bitInvertSourcewith). */
/* BitBltSimulation>>#bitInvertSource:with: */

static unsigned int bitInvertSourcewith(unsigned int sourceWord,
                                        unsigned int destinationWord) {
  return (unsigned int)~sourceWord;
}
