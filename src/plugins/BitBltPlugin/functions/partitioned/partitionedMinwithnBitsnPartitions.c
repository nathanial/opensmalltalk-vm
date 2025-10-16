/* Extracted from BitBltPlugin.c:3885 (function partitionedMinwithnBitsnPartitions). */
/* BitBltSimulation>>#partitionedMin:with:nBits:nPartitions: */
/*	Multiply each channel of nBits in word1 and word2.
        We assume that for each channel of nBits, we multiply ratios in interval
        [0..1], scaled by (1 << nBits - 1).
        result := ((channel1/scale) * (channel2/scale) * scale) rounded
        Or after simplification:
        result := (channel1 * channel2 / scale) rounded
        This is implemented by first forming the double precision products
        (channel1 * channel2) on a double-word.
        Then dividing each double precision channel by scale, with correctly
        rounded operation.
        With proper tricks, some of these operations can be multiplexed
        (all channels are formed in parallel with a single sequence of
   operation).
 */

static unsigned int partitionedMinwithnBitsnPartitions(unsigned int word1,
                                                       unsigned int word2,
                                                       sqInt nBits,
                                                       sqInt nParts) {
  sqInt i;
  unsigned int mask;
  unsigned int result;

  if (nBits == 32) {
    result = ((word2 < word1) ? word2 : word1);
  } else {
    result = 0;

    /* partition mask starts at the right */
    mask = maskTable[nBits];
    for (i = 1; i <= nParts; i += 1) {
      result = result | ((((word2 & mask) < (word1 & mask)) ? (word2 & mask)
                                                            : (word1 & mask)));

      /* slide left to next partition */
      mask = (((usqInt)(mask) << nBits));
    }
  }
  return result;
}
