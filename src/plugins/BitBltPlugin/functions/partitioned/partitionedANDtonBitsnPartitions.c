/* Extracted from BitBltPlugin.c:3813 (function partitionedANDtonBitsnPartitions). */
/* BitBltSimulation>>#partitionedAND:to:nBits:nPartitions: */
/*	Max word1 to word2 as nParts partitions of nBits each */
/*	In C, most arithmetic operations answer the same bit pattern regardless
   of the operands being signed or unsigned ints (this is due to the way 2's
   complement numbers work). However, comparisions might fail. Add the proper
   declaration of words as unsigned int in those cases where comparisions are
   done (jmv)
 */

static unsigned int partitionedANDtonBitsnPartitions(unsigned int word1,
                                                     unsigned int word2,
                                                     sqInt nBits,
                                                     sqInt nParts) {
  sqInt i;
  unsigned int mask;
  unsigned int result;

  /* partition mask starts at the right */
  mask = maskTable[nBits];
  result = 0;
  if (nBits == 32) {
    if (word1 == mask) {
      result = result | word2;
    }
  } else {
    for (i = 1; i <= nParts; i += 1) {
      if ((word1 & mask) == mask) {
        result = result | (word2 & mask);
      }

      /* slide left to next partition */
      mask = (((usqInt)(mask) << nBits));
    }
  }
  return result;
}
