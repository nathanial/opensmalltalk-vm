/* Extracted from BitBltPlugin.c:5546 (function rgbDiffwith). */

static unsigned int rgbDiffwith(unsigned int sourceWord,
                                unsigned int destinationWord) {
  sqInt bitsPerColor;
  unsigned int destPixVal;
  unsigned int destShifted;
  unsigned int diff;
  sqInt i;
  unsigned int maskShifted;
  unsigned int pixMask;
  unsigned int rgbMask;
  unsigned int sourcePixVal;
  unsigned int sourceShifted;

  pixMask = maskTable[destDepth];
  if (destDepth == 16) {
    bitsPerColor = 5;
    rgbMask = 0x1F;
  } else {
    bitsPerColor = 8;
    rgbMask = 0xFF;
  }
  maskShifted = destMask;
  destShifted = destinationWord;
  sourceShifted = sourceWord;
  for (i = 1; i <= destPPW; i += 1) {
    if ((maskShifted & pixMask) > 0) {
      destPixVal = destShifted & pixMask;
      sourcePixVal = sourceShifted & pixMask;
      if (destDepth < 16) {
        if (sourcePixVal == destPixVal) {
          diff = 0;
        } else {
          diff = 1;
        }
      } else {
        diff = partitionedSubfromnBitsnPartitions(sourcePixVal, destPixVal,
                                                  bitsPerColor, 3);
        diff =
            ((diff & rgbMask) +
             ((((usqInt)(diff)) >> bitsPerColor) & rgbMask)) +
            ((((usqInt)((((usqInt)(diff)) >> bitsPerColor))) >> bitsPerColor) &
             rgbMask);
      }
      bitCount += diff;
    }

    /* Only tally pixels within the destination rectangle */
    maskShifted = ((usqInt)(maskShifted)) >> destDepth;
    sourceShifted = ((usqInt)(sourceShifted)) >> destDepth;
    destShifted = ((usqInt)(destShifted)) >> destDepth;
  }
  return destinationWord;
}