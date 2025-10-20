/* Extracted from BitBltPlugin.c:3541 (function OLDrgbDiffwith). */
/* BitBltSimulation>>#OLDrgbDiff:with: */
/*	Tally pixels into the color map. Note that the source should be
        specified = destination, in order for the proper color map checks
        to be performed at setup.
        Note that the region is not clipped to bit boundaries, but only to the
        nearest (enclosing) word. This is because copyLoop does not do
        pre-merge masking. For accurate results, you must subtract the
        values obtained from the left and right fringes. */

static sqInt OLDrgbDiffwith(sqInt sourceWord, sqInt destinationWord) {
  sqInt diff;
  int pixMask;

  if (destDepth < 16) {
    diff = sourceWord ^ destinationWord;
    pixMask = maskTable[destDepth];
    while (!(diff == 0)) {
      if (diff & pixMask) {
        bitCount += 1;
      }
      diff = ((usqInt)(diff)) >> destDepth;
    }
    return destinationWord;
  }

  /* Just xor and count differing bits if not RGB */
  if (destDepth == 16) {
    diff =
        partitionedSubfromnBitsnPartitions(sourceWord, destinationWord, 5, 3);
    bitCount = ((bitCount + (diff & 0x1F)) + ((((usqInt)(diff)) >> 5) & 0x1F)) +
               ((((usqInt)(diff)) >> 10) & 0x1F);
    diff = partitionedSubfromnBitsnPartitions(
        ((usqInt)(sourceWord)) >> 16, ((usqInt)(destinationWord)) >> 16, 5, 3);
    bitCount = ((bitCount + (diff & 0x1F)) + ((((usqInt)(diff)) >> 5) & 0x1F)) +
               ((((usqInt)(diff)) >> 10) & 0x1F);
  } else {
    diff =
        partitionedSubfromnBitsnPartitions(sourceWord, destinationWord, 8, 3);
    bitCount = ((bitCount + (diff & 0xFF)) + ((((usqInt)(diff)) >> 8) & 0xFF)) +
               ((((usqInt)(diff)) >> 16) & 0xFF);
  }
  return destinationWord;
}
