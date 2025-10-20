/* Extracted from BitBltPlugin.c:2420 (function copyLoopNoSource). */

static sqInt copyLoopNoSource(void) {
  unsigned int destWord;
  unsigned int halftoneWord;
  sqInt i;
  unsigned int (*mergeFnwith)(unsigned int, unsigned int);
  unsigned int mergeWord;
  sqInt word;

  halftoneWord = 0;
  mergeFnwith = ((unsigned int (*)(unsigned int, unsigned int))(
      opTable[combinationRule + 1]));
  if (noHalftone) {
    halftoneWord = AllOnes;
  }
  for (i = 1; i <= bbH; i += 1) {
    if (!noHalftone) {
      halftoneWord = long32At(
          (void *)(halftoneBase + ((((dy + i) - 1) % halftoneHeight) * 4)));
    }

    /* Note: the horizontal loop has been expanded into three parts for speed:
       This first section requires masking of the destination store... */
    destMask = mask1;

    /* begin dstLongAt: */
    assert((((usqInt)destIndex)) < endOfDestination);
    destWord = long32At((void *)(destIndex));
    mergeWord = mergeFnwith(halftoneWord, destWord);
    destWord = (destMask & mergeWord) | (destWord & ((unsigned int)~destMask));

    /* dstLongAt:put: */
    long32Atput((void *)(destIndex), destWord);

    /* incDestIndex: */
    destIndex += 4;

    /* This central horizontal loop requires no store masking */
    destMask = AllOnes;
    if (combinationRule == 3) {
      destWord = halftoneWord;
      for (word = 2; word < nWords; word += 1) {
        /* dstLongAt:put: */
        long32Atput((void *)(destIndex), destWord);

        /* incDestIndex: */
        destIndex += 4;
      }
    } else {
      for (word = 2; word < nWords; word += 1) {
        /* begin dstLongAt: */
        assert((((usqInt)destIndex)) < endOfDestination);
        destWord = long32At((void *)(destIndex));
        mergeWord = mergeFnwith(halftoneWord, destWord);

        /* dstLongAt:put: */
        long32Atput((void *)(destIndex), mergeWord);

        /* incDestIndex: */
        destIndex += 4;
      }
    }

    /* Special inner loop for STORE
       Normal inner loop does merge
       This last section, if used, requires masking of the destination store...
     */
    if (nWords > 1) {
      destMask = mask2;

      /* begin dstLongAt: */
      assert((((usqInt)destIndex)) < endOfDestination);
      destWord = long32At((void *)(destIndex));
      mergeWord = mergeFnwith(halftoneWord, destWord);
      destWord =
          (destMask & mergeWord) | (destWord & ((unsigned int)~destMask));

      /* dstLongAt:put: */
      long32Atput((void *)(destIndex), destWord);

      /* incDestIndex: */
      destIndex += 4;
    }

    /* incDestIndex: */
    destIndex += destDelta;
  }
  return 0;
}