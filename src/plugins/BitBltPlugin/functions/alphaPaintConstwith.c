/* Extracted from BitBltPlugin.c:726 (function alphaPaintConstwith). */

static unsigned int alphaPaintConstwith(unsigned int sourceWord,
                                        unsigned int destinationWord) {
  if (!sourceWord) {
    return destinationWord;
  }
  return alphaBlendConstwithpaintMode(sourceWord, destinationWord, 1);
}