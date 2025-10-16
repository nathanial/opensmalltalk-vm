/* Extracted from BitBltPlugin.c:3787 (function partitionedAddtonBitscomponentMaskcarryOverflowMask). */
/* BitBltSimulation>>#partitionedAdd:to:nBits:componentMask:carryOverflowMask: */
/*	AND word1 to word2 as nParts partitions of nBits each.
        Any field of word1 not all-ones is treated as all-zeroes.
        Used for erasing, eg, brush shapes prior to ORing in a color */

static unsigned int partitionedAddtonBitscomponentMaskcarryOverflowMask(
    unsigned int word1, unsigned int word2, sqInt nBits,
    unsigned int componentMask, unsigned int carryOverflowMask) {
  unsigned int carryOverflow;
  unsigned int sum;
  unsigned int w1;
  unsigned int w2;

  /* mask to remove high bit of each component */
  w1 = word1 & carryOverflowMask;
  w2 = word2 & carryOverflowMask;

  /* sum without high bit to avoid overflowing over next component */
  sum = (word1 ^ w1) + (word2 ^ w2);

  /* detect overflow condition for saturating */
  carryOverflow = (w1 & w2) | ((w1 | w2) & sum);
  return ((sum ^ w1) ^ w2) |
         ((((usqInt)(carryOverflow)) >> (nBits - 1)) * componentMask);
}
