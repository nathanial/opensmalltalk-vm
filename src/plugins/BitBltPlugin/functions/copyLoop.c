/* Extracted from BitBltPlugin.c:2070 (function copyLoop). */

static sqInt copyLoop(void) {
  unsigned int destWord;
  unsigned int halftoneWord;
  sqInt hInc;
  sqInt i;
  unsigned int (*mergeFnwith)(unsigned int, unsigned int);
  unsigned int mergeWord;
  unsigned int notSkewMask;
  unsigned int prevWord;
  unsigned int skewMask;
  unsigned int skewWord;
  unsigned int thisWord;
  int unskew;
  sqInt word;
  sqInt y;

  /* unskew is a bitShift and MUST remain signed, while skewMask is unsigned. */
  mergeFnwith = ((unsigned int (*)(unsigned int, unsigned int))(
      opTable[combinationRule + 1]));
  assert(!((preload && (skew == 0))));
  assert(((skew >= -31) && (skew <= 0x1F)));

  /* Byte delta */
  hInc = hDir * 4;
  if (skew < 0) {
    unskew = skew + 32;
    skewMask = (((usqInt)(AllOnes) << (0 - skew)));
  } else {
    if (skew) {
      unskew = skew - 32;
      skewMask = ((usqInt)(AllOnes)) >> skew;
    } else {
      unskew = 0;
      skewMask = AllOnes;
    }
  }
  notSkewMask = (unsigned int)~skewMask;
  if (noHalftone) {
    halftoneWord = AllOnes;
    halftoneHeight = 0;
  } else {
    halftoneWord =
        long32At((void *)(halftoneBase + ((0 % halftoneHeight) * 4)));
  }
  y = dy;

  /* Here is the vertical loop, in two versions, one for the combinationRule = 3
   * copy mode, one for the general case. */
  if (combinationRule == 3) {
    for (i = 1; i <= bbH; i += 1) {
      if (halftoneHeight > 1) {
        halftoneWord =
            long32At((void *)(halftoneBase + ((y % halftoneHeight) * 4)));
        y += vDir;
      }

      /* Otherwise, its always the same */
      if (preload) {
        /* begin srcLongAt: */
        assert((((usqInt)sourceIndex)) < endOfSource);
        prevWord = long32At((void *)(sourceIndex));

        /* incSrcIndex: */
        sourceIndex += hInc;
      } else {
        prevWord = 0;
      }

      /* load the 64-bit shifter
         Note: the horizontal loop has been expanded into three parts for speed:
         This first section requires masking of the destination store... */
      destMask = mask1;

      /* begin srcLongAt: */
      assert((((usqInt)sourceIndex)) < endOfSource);

      /* pick up next word */
      thisWord = long32At((void *)(sourceIndex));

      /* incSrcIndex: */
      sourceIndex += hInc;
      skewWord = (((unskew < 0)
                       ? (((usqInt)((prevWord & notSkewMask))) >> (-unskew))
                       : ((((usqInt)((prevWord & notSkewMask)) << unskew))))) |
                 (((skew < 0) ? (((usqInt)((thisWord & skewMask))) >> (-skew))
                              : ((((usqInt)((thisWord & skewMask)) << skew)))));

      /* 32-bit rotate */
      prevWord = thisWord;

      /* begin dstLongAt: */
      assert((((usqInt)destIndex)) < endOfDestination);
      destWord = long32At((void *)(destIndex));
      destWord = (destMask & (skewWord & halftoneWord)) |
                 (destWord & ((unsigned int)~destMask));

      /* dstLongAt:put: */
      long32Atput((void *)(destIndex), destWord);

      /* incDestIndex: */
      destIndex += hInc;

      /* This central horizontal loop requires no store masking */
      destMask = AllOnes;
      if ((skew == 0) && (halftoneWord == AllOnes)) {
        if (preload && (hDir == 1)) {
          for (word = 2; word < nWords; word += 1) {
            /* dstLongAt:put: */
            long32Atput((void *)(destIndex), prevWord);

            /* incDestIndex: */
            destIndex += hInc;

            /* begin srcLongAt: */
            assert((((usqInt)sourceIndex)) < endOfSource);
            prevWord = long32At((void *)(sourceIndex));

            /* incSrcIndex: */
            sourceIndex += hInc;
          }
        } else {
          for (word = 2; word < nWords; word += 1) {
            /* begin srcLongAt: */
            assert((((usqInt)sourceIndex)) < endOfSource);
            thisWord = long32At((void *)(sourceIndex));

            /* incSrcIndex: */
            sourceIndex += hInc;

            /* dstLongAt:put: */
            long32Atput((void *)(destIndex), thisWord);

            /* incDestIndex: */
            destIndex += hInc;
          }
          prevWord = thisWord;
        }
      } else {
        for (word = 2; word < nWords; word += 1) {
          /* begin srcLongAt: */
          assert((((usqInt)sourceIndex)) < endOfSource);
          thisWord = long32At((void *)(sourceIndex));

          /* incSrcIndex: */
          sourceIndex += hInc;
          skewWord =
              (((unskew < 0)
                    ? (((usqInt)((prevWord & notSkewMask))) >> (-unskew))
                    : ((((usqInt)((prevWord & notSkewMask)) << unskew))))) |
              (((skew < 0) ? (((usqInt)((thisWord & skewMask))) >> (-skew))
                           : ((((usqInt)((thisWord & skewMask)) << skew)))));

          /* 32-bit rotate */
          prevWord = thisWord;

          /* dstLongAt:put: */
          long32Atput((void *)(destIndex), skewWord & halftoneWord);

          /* incDestIndex: */
          destIndex += hInc;
        }
      }

      /* Very special inner loop for STORE mode with no skew -- just move words
         This last section, if used, requires masking of the destination
         store... */
      if (nWords > 1) {
        destMask = mask2;
        thisWord = ((((skew < 0) ? (((usqInt)(skewMask)) >> (-skew))
                                 : ((((usqInt)(skewMask) << skew))))) &
                            mask2
                        ? (/* begin srcLongAt: */
                           assert((((usqInt)sourceIndex)) < endOfSource),
                           long32At((void *)(sourceIndex)))
                        : 0);

        /* we don't need more bits, they will all come from prevWord */

        /* incSrcIndex: */
        sourceIndex += hInc;
        skewWord =
            (((unskew < 0)
                  ? (((usqInt)((prevWord & notSkewMask))) >> (-unskew))
                  : ((((usqInt)((prevWord & notSkewMask)) << unskew))))) |
            (((skew < 0) ? (((usqInt)((thisWord & skewMask))) >> (-skew))
                         : ((((usqInt)((thisWord & skewMask)) << skew)))));

        /* 32-bit rotate */

        /* begin dstLongAt: */
        assert((((usqInt)destIndex)) < endOfDestination);
        destWord = long32At((void *)(destIndex));
        destWord = (destMask & (skewWord & halftoneWord)) |
                   (destWord & ((unsigned int)~destMask));

        /* dstLongAt:put: */
        long32Atput((void *)(destIndex), destWord);

        /* incDestIndex: */
        destIndex += hInc;
      }

      /* incSrcIndex: */
      sourceIndex += sourceDelta;

      /* incDestIndex: */
      destIndex += destDelta;
    }
  } else {
    for (i = 1; i <= bbH; i += 1) {
      if (halftoneHeight > 1) {
        halftoneWord =
            long32At((void *)(halftoneBase + ((y % halftoneHeight) * 4)));
        y += vDir;
      }

      /* Otherwise, its always the same */
      if (preload) {
        /* begin srcLongAt: */
        assert((((usqInt)sourceIndex)) < endOfSource);
        prevWord = long32At((void *)(sourceIndex));

        /* incSrcIndex: */
        sourceIndex += hInc;
      } else {
        prevWord = 0;
      }

      /* load the 64-bit shifter
         Note: the horizontal loop has been expanded into three parts for speed:
         This first section requires masking of the destination store... */
      destMask = mask1;

      /* begin srcLongAt: */
      assert((((usqInt)sourceIndex)) < endOfSource);

      /* pick up next word */
      thisWord = long32At((void *)(sourceIndex));

      /* incSrcIndex: */
      sourceIndex += hInc;
      skewWord = (((unskew < 0)
                       ? (((usqInt)((prevWord & notSkewMask))) >> (-unskew))
                       : ((((usqInt)((prevWord & notSkewMask)) << unskew))))) |
                 (((skew < 0) ? (((usqInt)((thisWord & skewMask))) >> (-skew))
                              : ((((usqInt)((thisWord & skewMask)) << skew)))));

      /* 32-bit rotate */
      prevWord = thisWord;

      /* begin dstLongAt: */
      assert((((usqInt)destIndex)) < endOfDestination);
      destWord = long32At((void *)(destIndex));
      mergeWord = mergeFnwith(skewWord & halftoneWord, destWord);
      destWord =
          (destMask & mergeWord) | (destWord & ((unsigned int)~destMask));

      /* dstLongAt:put: */
      long32Atput((void *)(destIndex), destWord);

      /* incDestIndex: */
      destIndex += hInc;

      /* This central horizontal loop requires no store masking */
      destMask = AllOnes;
      for (word = 2; word < nWords; word += 1) {
        /* begin srcLongAt: */
        assert((((usqInt)sourceIndex)) < endOfSource);

        /* pick up next word */
        thisWord = long32At((void *)(sourceIndex));

        /* incSrcIndex: */
        sourceIndex += hInc;
        skewWord =
            (((unskew < 0)
                  ? (((usqInt)((prevWord & notSkewMask))) >> (-unskew))
                  : ((((usqInt)((prevWord & notSkewMask)) << unskew))))) |
            (((skew < 0) ? (((usqInt)((thisWord & skewMask))) >> (-skew))
                         : ((((usqInt)((thisWord & skewMask)) << skew)))));

        /* 32-bit rotate */
        prevWord = thisWord;
        mergeWord =
            mergeFnwith(skewWord & halftoneWord,
                        (/* begin dstLongAt: */
                         assert((((usqInt)destIndex)) < endOfDestination),
                         long32At((void *)(destIndex))));

        /* dstLongAt:put: */
        long32Atput((void *)(destIndex), mergeWord);

        /* incDestIndex: */
        destIndex += hInc;
      }

      /* Normal inner loop does merge:
         This last section, if used, requires masking of the destination
         store... */
      if (nWords > 1) {
        destMask = mask2;
        thisWord = ((((skew < 0) ? (((usqInt)(skewMask)) >> (-skew))
                                 : ((((usqInt)(skewMask) << skew))))) &
                            mask2
                        ? (/* begin srcLongAt: */
                           assert((((usqInt)sourceIndex)) < endOfSource),
                           long32At((void *)(sourceIndex)))
                        : 0);

        /* we don't need more bits, they will all come from prevWord */

        /* incSrcIndex: */
        sourceIndex += hInc;
        skewWord =
            (((unskew < 0)
                  ? (((usqInt)((prevWord & notSkewMask))) >> (-unskew))
                  : ((((usqInt)((prevWord & notSkewMask)) << unskew))))) |
            (((skew < 0) ? (((usqInt)((thisWord & skewMask))) >> (-skew))
                         : ((((usqInt)((thisWord & skewMask)) << skew)))));

        /* 32-bit rotate */

        /* begin dstLongAt: */
        assert((((usqInt)destIndex)) < endOfDestination);
        destWord = long32At((void *)(destIndex));
        mergeWord = mergeFnwith(skewWord & halftoneWord, destWord);
        destWord =
            (destMask & mergeWord) | (destWord & ((unsigned int)~destMask));

        /* dstLongAt:put: */
        long32Atput((void *)(destIndex), destWord);

        /* incDestIndex: */
        destIndex += hInc;
      }

      /* incSrcIndex: */
      sourceIndex += sourceDelta;

      /* incDestIndex: */
      destIndex += destDelta;
    }
  }
  return 0;
}