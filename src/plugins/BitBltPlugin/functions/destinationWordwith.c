/* Extracted from BitBltPlugin.c:2830 (function destinationWordwith). */
/* BitBltSimulation>>#destinationWord:with: */
/*	Return the integer value of the given field of the given object. If the
        field contains a Float, truncate it and return its integral part. Fail
   if the given field does not contain a small integer or Float, or if the
        truncated Float is out of the range of small integers.
 */

static unsigned int destinationWordwith(unsigned int sourceWord,
                                        unsigned int destinationWord) {
  return destinationWord;
}
