/* Extracted from BitBltPlugin.c:5752 (function rgbSubwith). */

static unsigned int rgbSubwith(unsigned int sourceWord,
                               unsigned int destinationWord) {
  if (destDepth < 16) {
    return partitionedSubfromnBitsnPartitions(sourceWord, destinationWord,
                                              destDepth, destPPW);
  }

  /* Sub each pixel separately */
  if (destDepth == 16) {
    return (partitionedSubfromnBitsnPartitions(sourceWord, destinationWord, 5,
                                               3)) +
           ((((usqInt)((partitionedSubfromnBitsnPartitions(
                  ((usqInt)(sourceWord)) >> 16,
                  ((usqInt)(destinationWord)) >> 16, 5, 3)))
              << 16)));
  } else {
    return partitionedSubfromnBitsnPartitions(sourceWord, destinationWord, 8,
                                              4);
  }
}