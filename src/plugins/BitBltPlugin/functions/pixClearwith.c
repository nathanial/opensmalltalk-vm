/* Extracted from BitBltPlugin.c:4055 (function pixClearwith). */

static unsigned int pixClearwith(unsigned int sourceWord,
                                 unsigned int destinationWord) {
  sqInt i;
  unsigned int mask;
  int nBits;
  unsigned int pv;
  unsigned int result;

  if (destDepth == 32) {
    if (sourceWord == destinationWord) {
      return 0;
    } else {
      return destinationWord;
    }
  }
  nBits = destDepth;

  /* partition mask starts at the right */
  mask = maskTable[nBits];
  result = 0;
  for (i = 1; i <= destPPW; i += 1) {
    pv = destinationWord & mask;
    if ((sourceWord & mask) == pv) {
      pv = 0;
    }
    result = result | pv;

    /* slide left to next partition */
    mask = (((usqInt)(mask) << nBits));
  }
  return result;
}