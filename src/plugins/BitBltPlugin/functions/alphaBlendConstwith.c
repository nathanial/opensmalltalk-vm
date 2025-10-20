/* Extracted from BitBltPlugin.c:452 (function alphaBlendConstwith). */

static unsigned int alphaBlendConstwith(unsigned int sourceWord,
                                        unsigned int destinationWord) {
  return alphaBlendConstwithpaintMode(sourceWord, destinationWord, 0);
}