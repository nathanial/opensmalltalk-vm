/* Extracted from BitBltPlugin.c:4014 (function partitionedSubfromnBitsnPartitions). */
/* BitBltSimulation>>#partitionedSub:from:nBits:nPartitions: */
/*	Clear all pixels in destinationWord for which the pixels of sourceWord
        have the same values. Used to clear areas of some constant color to
   zero.
 */

static unsigned int partitionedSubfromnBitsnPartitions(unsigned int word1,
                                                       unsigned int word2,
                                                       sqInt nBits,
                                                       sqInt nParts) {
  sqInt i;
  unsigned int mask;
  unsigned int p1;
  unsigned int p2;
  unsigned int result;

  if (nBits == 32) {
    result = (word1 < word2 ? word2 - word1 : word1 - word2);
  } else {
    result = 0;

    /* partition mask starts at the right */
    mask = maskTable[nBits];
    for (i = 1; i <= nParts; i += 1) {
      p1 = word1 & mask;
      p2 = word2 & mask;
      if (p1 < p2) {
        result = result | (p2 - p1);
      } else {
        result = result | (p1 - p2);
      }

      /* result is really abs value of thedifference */

      /* slide left to next partition */
      mask = (((usqInt)(mask) << nBits));
    }
  }
  return result;
}
