/* Extracted from BitBltPlugin.c:2868 (function fixAlphawith). */
/* BitBltSimulation>>#fixAlpha:with: */
/*	Note: This is hardcoded so it can be run from Squeak.
        The module name is used for validating a module *after*
        it is loaded to check if it does really contain the module
        we're thinking it contains. This is important! */

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
