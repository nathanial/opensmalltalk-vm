/* Extracted from BitBltPlugin.c:4096 (function pixPaintwith). */

static unsigned int pixPaintwith(unsigned int sourceWord,
                                 unsigned int destinationWord) {
  if (!sourceWord) {
    return destinationWord;
  }
  return sourceWord |
         (partitionedANDtonBitsnPartitions(
             (unsigned int)~sourceWord, destinationWord, destDepth, destPPW));
}