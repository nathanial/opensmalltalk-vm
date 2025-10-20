/* Extracted from BitBltPlugin.c:2868 (function fixAlphawith). */

static unsigned int fixAlphawith(unsigned int sourceWord,
                                 unsigned int destinationWord) {
  if (!(destDepth == 32)) {
    return destinationWord;
  }
  if (!destinationWord) {
    return 0;
  }
  if (destinationWord & 0xFF000000U) {
    return destinationWord;
  }
  return destinationWord | (sourceWord & 0xFF000000U);
}