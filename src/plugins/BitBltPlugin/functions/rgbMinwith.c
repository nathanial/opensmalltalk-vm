/* Extracted from BitBltPlugin.c:5706 (function rgbMinwith). */
/* BitBltSimulation>>#rgbMin:with: */

static unsigned int rgbMinwith(unsigned int sourceWord,
                               unsigned int destinationWord) {
  if (destDepth < 16) {
    return partitionedMinwithnBitsnPartitions(sourceWord, destinationWord,
                                              destDepth, destPPW);
  }

  /* Min each pixel separately */
  if (destDepth == 16) {
    return (partitionedMinwithnBitsnPartitions(sourceWord, destinationWord, 5,
                                               3)) +
           ((((usqInt)((partitionedMinwithnBitsnPartitions(
                  ((usqInt)(sourceWord)) >> 16,
                  ((usqInt)(destinationWord)) >> 16, 5, 3)))
              << 16)));
  } else {
    return partitionedMinwithnBitsnPartitions(sourceWord, destinationWord, 8,
                                              4);
  }
}
