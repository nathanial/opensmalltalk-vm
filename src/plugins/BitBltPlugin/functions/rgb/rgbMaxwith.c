/* Extracted from BitBltPlugin.c:5659 (function rgbMaxwith). */
/* BitBltSimulation>>#rgbMax:with: */

static unsigned int rgbMaxwith(unsigned int sourceWord,
                               unsigned int destinationWord) {
  if (destDepth < 16) {
    return partitionedMaxwithnBitsnPartitions(sourceWord, destinationWord,
                                              destDepth, destPPW);
  }

  /* Max each pixel separately */
  if (destDepth == 16) {
    return (partitionedMaxwithnBitsnPartitions(sourceWord, destinationWord, 5,
                                               3)) +
           ((((usqInt)((partitionedMaxwithnBitsnPartitions(
                  ((usqInt)(sourceWord)) >> 16,
                  ((usqInt)(destinationWord)) >> 16, 5, 3)))
              << 16)));
  } else {
    return partitionedMaxwithnBitsnPartitions(sourceWord, destinationWord, 8,
                                              4);
  }
}
