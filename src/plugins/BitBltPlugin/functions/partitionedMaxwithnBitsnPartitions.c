/* Extracted from BitBltPlugin.c:3850 (function partitionedMaxwithnBitsnPartitions). */

static unsigned int partitionedMaxwithnBitsnPartitions(unsigned int word1,
                                                       unsigned int word2,
                                                       sqInt nBits,
                                                       sqInt nParts) {
  sqInt i;
  unsigned int mask;
  unsigned int result;

  if (nBits == 32) {
    result = ((word2 < word1) ? word1 : word2);
  } else {
    result = 0;

    /* partition mask starts at the right */
    mask = maskTable[nBits];
    for (i = 1; i <= nParts; i += 1) {
      result = result | ((((word2 & mask) < (word1 & mask)) ? (word1 & mask)
                                                            : (word2 & mask)));

      /* slide left to next partition */
      mask = (((usqInt)(mask) << nBits));
    }
  }
  return result;
}