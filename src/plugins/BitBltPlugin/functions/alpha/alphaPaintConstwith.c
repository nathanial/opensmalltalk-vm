/* Extracted from BitBltPlugin.c:726 (function alphaPaintConstwith). */
/* BitBltSimulation>>#alphaPaintConst:with: */
/*	Scale (premultiply) the destination with its alpha channel.
        Note that sourceWord is ignored. */

static unsigned int alphaPaintConstwith(unsigned int sourceWord,
                                        unsigned int destinationWord) {
  if (!sourceWord) {
    return destinationWord;
  }
  return alphaBlendConstwithpaintMode(sourceWord, destinationWord, 1);
}
