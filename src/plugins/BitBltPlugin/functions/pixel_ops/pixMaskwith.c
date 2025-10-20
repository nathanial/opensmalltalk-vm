/* Extracted from BitBltPlugin.c:4089 (function pixMaskwith). */
/* BitBltSimulation>>#pixMask:with: */

static unsigned int pixMaskwith(unsigned int sourceWord,
                                unsigned int destinationWord) {
  return partitionedANDtonBitsnPartitions((unsigned int)~sourceWord,
                                          destinationWord, destDepth, destPPW);
}
