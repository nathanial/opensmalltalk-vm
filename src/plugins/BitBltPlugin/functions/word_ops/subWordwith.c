/* Extracted from BitBltPlugin.c:5952 (function subWordwith). */
/* BitBltSimulation>>#subWord:with: */
/*	Tally pixels into the color map. Those tallied are exactly those
        in the destination rectangle. Note that the source should be
        specified == destination, in order for the proper color map checks
        to be performed at setup. */

static unsigned int subWordwith(unsigned int sourceWord,
                                unsigned int destinationWord) {
  return sourceWord - destinationWord;
}
