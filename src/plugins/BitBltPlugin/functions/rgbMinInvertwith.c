/* Extracted from BitBltPlugin.c:5681 (function rgbMinInvertwith). */

static unsigned int rgbMinInvertwith(unsigned int wordToInvert,
                                     unsigned int destinationWord) {
  unsigned int sourceWord;

  sourceWord = (unsigned int)~wordToInvert;
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