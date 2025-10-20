/* Extracted from BitBltPlugin.c:5728 (function rgbMulwith). */

static unsigned int rgbMulwith(unsigned int sourceWord,
                               unsigned int destinationWord) {
  if (destDepth < 16) {
    if (destDepth == 1) {
      return sourceWord & destinationWord;
    }
    return partitionedMulwithnBitswordBits(sourceWord, destinationWord,
                                           destDepth, 32);
  }

  /* Mul each pixel separately */
  if (destDepth == 16) {
    return (partitionedMulwithnBitswordBits(sourceWord & 0xFFFF,
                                            destinationWord & 0xFFFF, 5, 16)) +
           ((((usqInt)((partitionedMulwithnBitswordBits(
                  ((usqInt)(sourceWord)) >> 16,
                  ((usqInt)(destinationWord)) >> 16, 5, 16)))
              << 16)));
  } else {
    return partitionedMulwithnBitswordBits(sourceWord, destinationWord, 8, 32);
  }
}