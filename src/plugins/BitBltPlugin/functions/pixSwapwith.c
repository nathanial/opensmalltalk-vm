/* Extracted from BitBltPlugin.c:4109 (function pixSwapwith). */

static unsigned int pixSwapwith(unsigned int sourceWord,
                                unsigned int destWord) {
  unsigned int highMask;
  sqInt i;
  unsigned int lowMask;
  unsigned int result;
  int shift;

  if (destPPW == 1) {
    return destWord;
  }
  result = 0;

  /* mask low pixel */
  lowMask = (1U << destDepth) - 1;

  /* mask high pixel */
  highMask = (((usqInt)(lowMask) << ((destPPW - 1) * destDepth)));
  shift = 32 - destDepth;
  result = result | (((((usqInt)((destWord & lowMask)) << shift))) |
                     (((usqInt)((destWord & highMask))) >> shift));
  if (destPPW <= 2) {
    return result;
  }
  for (i = 2; i <= (destPPW / 2); i += 1) {
    lowMask = (((usqInt)(lowMask) << destDepth));
    highMask = ((usqInt)(highMask)) >> destDepth;
    shift -= destDepth * 2;
    result = result | (((((usqInt)((destWord & lowMask)) << shift))) |
                       (((usqInt)((destWord & highMask))) >> shift));
  }
  return result;
}