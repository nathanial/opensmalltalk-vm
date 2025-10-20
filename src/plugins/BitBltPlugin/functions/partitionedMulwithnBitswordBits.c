/* Extracted from BitBltPlugin.c:3927 (function partitionedMulwithnBitswordBits). */

static unsigned int partitionedMulwithnBitswordBits(unsigned int word1,
                                                    unsigned int word2,
                                                    sqInt nBits,
                                                    sqInt wordBits) {
  unsigned int channelMask;
  unsigned long long doubleGroupMask;
  unsigned long long doubleWord1;
  unsigned long long doubleWord2;
  unsigned long long doubleWordMul;
  unsigned int groupMask;
  unsigned int half;
  sqInt highWordShift;
  sqInt i;
  sqInt n2;
  sqInt nGroups;
  unsigned int result;
  sqInt shift;

  /* width of double-precision channel */
  n2 = 2 * nBits;

  /* partition mask starts at the right */
  channelMask = (1U << nBits) - 1;

  /* number of channels that fit in a word, when alternating with group of zeros
   */
  nGroups = ((wordBits / nBits) + 1) / 2;

  /* form a word mask with alternate nBits 0 and nBits 1, so as to select even
   * channels */
  groupMask = channelMask;
  for (i = 2; i <= nGroups; i += 1) {
    groupMask = ((((usqInt)(groupMask) << n2))) + channelMask;
  }

  /* shift for putting odd channels in high-word - usually wordBits, except if
   * wordBits \\ nBits ~= 0 */
  highWordShift = nGroups * n2;

  /* select odd channel interleaved with groups of nBits zeros, so as to leave
   * room for double-precision multiplication */
  doubleWord1 = (((usqInt)(word1)) >> nBits) & groupMask;
  doubleWord2 = (((usqInt)(word2)) >> nBits) & groupMask;

  /* Put odd channels in high word, and even channels in low word */
  doubleWord1 = ((doubleWord1 << highWordShift)) + (word1 & groupMask);
  doubleWord2 = ((doubleWord2 << highWordShift)) + (word2 & groupMask);

  /* mid-value to add for getting a correctly rounded division */
  half = (((usqInt)(channelMask)) >> 1) + 1;
  shift = 0;
  doubleWordMul = 0;
  for (i = 1; i <= (wordBits / nBits); i += 1) {
    /* multiply each channel of the two operands */
    doubleWordMul += ((((((doubleWord1) >> shift) & channelMask) *
                        (((doubleWord2) >> shift) & channelMask)) +
                       half)
                      << shift);
    shift += n2;
  }

  /* form a mask for extracting single-precision channels in the double word */
  doubleGroupMask = groupMask;
  doubleGroupMask = ((doubleGroupMask << highWordShift)) + groupMask;

  /* divide by scale */
  doubleWordMul =
      ((((((doubleWordMul) >> nBits) & doubleGroupMask) + doubleWordMul)) >>
       nBits) &
      doubleGroupMask;

  /* compact channels back into a single word */
  result = ((doubleWordMul) >> (highWordShift - nBits)) +
           (doubleWordMul & groupMask);
  return result;
}