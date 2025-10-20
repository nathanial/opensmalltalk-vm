/* Extracted from BitBltPlugin.c:2522 (function copyLoopPixMap). */

static sqInt copyLoopPixMap(void) {
  sqInt destPix;
  unsigned int destPixMask;
  unsigned int destWord;
  sqInt dstShift;
  int dstShiftInc;
  int dstShiftLeft;
  sqInt dstShiftSqInt;
  sqInt endBits;
  unsigned int halftoneWord;
  sqInt i;
  sqInt mapperFlags;
  unsigned int (*mergeFnwith)(unsigned int, unsigned int);
  unsigned int mergeWord;
  sqInt nPix;
  sqInt nPixSqInt;
  sqInt nSourceIncs;
  sqInt scrStartBits;
  unsigned int skewWord;
  sqInt sourcePix;
  unsigned int sourcePixMask;
  unsigned int sourceWord;
  sqInt srcShift;
  int srcShiftInc;
  sqInt srcShiftSqInt;
  sqInt startBits;
  sqInt val;
  sqInt words;

  halftoneWord = 0;
  mergeFnwith = ((unsigned int (*)(unsigned int, unsigned int))(
      opTable[combinationRule + 1]));
  sourcePPW = 32 / sourceDepth;
  sourcePixMask = maskTable[sourceDepth];
  destPixMask = maskTable[destDepth];
  mapperFlags = cmFlags & ((unsigned int)~ColorMapNewStyle);
  sourceIndex = (sourceBits + (sy * sourcePitch)) + ((sx / sourcePPW) * 4);
  scrStartBits = sourcePPW - (sx & (sourcePPW - 1));
  if (bbW < scrStartBits) {
    nSourceIncs = 0;
  } else {
    nSourceIncs = ((bbW - scrStartBits) / sourcePPW) + 1;
  }
  sourceDelta = sourcePitch - (nSourceIncs * 4);

  /* Note following two items were already calculated in destmask setup! */
  startBits = destPPW - (dx & (destPPW - 1));
  endBits = (((dx + bbW) - 1) & (destPPW - 1)) + 1;
  if (bbW < startBits) {
    startBits = bbW;
  }

  /* Precomputed shifts for pickSourcePixels */
  srcShift = (sx & (sourcePPW - 1)) * sourceDepth;
  dstShift = (dx & (destPPW - 1)) * destDepth;
  srcShiftInc = sourceDepth;
  dstShiftInc = destDepth;
  dstShiftLeft = 0;
  if (sourceMSB) {
    srcShift = (32 - sourceDepth) - srcShift;
    srcShiftInc = 0 - srcShiftInc;
  }
  if (destMSB) {
    dstShift = (32 - destDepth) - dstShift;
    dstShiftInc = 0 - dstShiftInc;
    dstShiftLeft = 32 - destDepth;
  }
  if (noHalftone) {
    halftoneWord = AllOnes;
  }
  for (i = 1; i <= bbH; i += 1) {
    if (!noHalftone) {
      halftoneWord = long32At(
          (void *)(halftoneBase + ((((dy + i) - 1) % halftoneHeight) * 4)));
    }

    /* setup first load */
    srcBitShift = srcShift;
    dstBitShift = dstShift;
    destMask = mask1;
    nPix = startBits;

    /* Here is the horizontal loop... */
    words = nWords;

    /* pick up the word */
    do {
      /* begin pickSourcePixels:flags:srcMask:destMask:srcShiftInc:dstShiftInc:
       */
      skewWord = 0;

      /* Hint: Keep in register */
      srcShiftSqInt = srcBitShift;

      /* Hint: Keep in register */
      dstShiftSqInt = dstBitShift;

      /* always > 0 so we can use do { } while(--nPix); */
      nPixSqInt = nPix;
      if (mapperFlags == (ColorMapPresent | ColorMapIndexedPart)) {
        do {
          /* begin srcLongAt: */
          assert((((usqInt)sourceIndex)) < endOfSource);
          sourceWord = long32At((void *)(sourceIndex));
          sourcePix = (((usqInt)(sourceWord)) >> srcShiftSqInt) & sourcePixMask;
          destPix = cmLookupTable[sourcePix & cmMask];
          skewWord = skewWord |
                     ((((usqInt)((destPix & destPixMask)) << dstShiftSqInt)));

          /* adjust dest pix index */
          dstShiftSqInt += dstShiftInc;

          /* adjust source pix index */
          if (((srcShiftSqInt += srcShiftInc)) & 0xFFFFFFE0U) {
            srcShiftSqInt =
                (sourceMSB ? srcShiftSqInt + 32 : srcShiftSqInt - 32);

            /* incSrcIndex: */
            sourceIndex += 4;
          }
        } while (!(((nPixSqInt -= 1)) == 0));
      } else {
        do {
          /* begin srcLongAt: */
          assert((((usqInt)sourceIndex)) < endOfSource);
          sourceWord = long32At((void *)(sourceIndex));
          sourcePix = (((usqInt)(sourceWord)) >> srcShiftSqInt) & sourcePixMask;

          /* begin mapPixel:flags: */
          destPix = sourcePix;
          if (mapperFlags & ColorMapPresent) {
            if (mapperFlags & ColorMapFixedPart) {
              /* begin rgbMapPixel:flags: */
              val = (((((int)(cmShiftTable[0]))) < 0)
                         ? (((usqInt)((sourcePix & (cmMaskTable[0])))) >>
                            (-(((int)(cmShiftTable[0])))))
                         : (((sqInt)((usqInt)((sourcePix & (cmMaskTable[0])))
                                     << (((int)(cmShiftTable[0])))))));
              val = val | ((((((int)(cmShiftTable[1]))) < 0)
                                ? (((usqInt)((sourcePix & (cmMaskTable[1])))) >>
                                   (-(((int)(cmShiftTable[1])))))
                                : ((((usqInt)((sourcePix & (cmMaskTable[1])))
                                     << (((int)(cmShiftTable[1]))))))));
              val = val | ((((((int)(cmShiftTable[2]))) < 0)
                                ? (((usqInt)((sourcePix & (cmMaskTable[2])))) >>
                                   (-(((int)(cmShiftTable[2])))))
                                : ((((usqInt)((sourcePix & (cmMaskTable[2])))
                                     << (((int)(cmShiftTable[2]))))))));
              destPix =
                  val | ((((((int)(cmShiftTable[3]))) < 0)
                              ? (((usqInt)((sourcePix & (cmMaskTable[3])))) >>
                                 (-(((int)(cmShiftTable[3])))))
                              : ((((usqInt)((sourcePix & (cmMaskTable[3])))
                                   << (((int)(cmShiftTable[3]))))))));

              /* avoid introducing transparency by color reduction */
              if ((destPix == 0) && (sourcePix != 0)) {
                destPix = 1;
              }
            }
            if (mapperFlags & ColorMapIndexedPart) {
              destPix = cmLookupTable[destPix & cmMask];
            }
          }
          skewWord = skewWord |
                     ((((usqInt)((destPix & destPixMask)) << dstShiftSqInt)));

          /* adjust dest pix index */
          dstShiftSqInt += dstShiftInc;

          /* adjust source pix index */
          if (((srcShiftSqInt += srcShiftInc)) & 0xFFFFFFE0U) {
            srcShiftSqInt =
                (sourceMSB ? srcShiftSqInt + 32 : srcShiftSqInt - 32);

            /* incSrcIndex: */
            sourceIndex += 4;
          }
        } while (!(((nPixSqInt -= 1)) == 0));
      }

      /* a little optimization for (pretty crucial) blits using indexed lookups
         only grab, colormap and mix in pixel grab, colormap and mix in pixel */

      /* Store back */
      srcBitShift = srcShiftSqInt;

      /* align next word to leftmost pixel */
      dstBitShift = dstShiftLeft;
      if (destMask == AllOnes) {
        mergeWord =
            mergeFnwith(skewWord & halftoneWord,
                        (/* begin dstLongAt: */
                         assert((((usqInt)destIndex)) < endOfDestination),
                         long32At((void *)(destIndex))));

        /* dstLongAt:put: */
        long32Atput((void *)(destIndex), destMask & mergeWord);
      } else {
        /* begin dstLongAt: */
        assert((((usqInt)destIndex)) < endOfDestination);
        destWord = long32At((void *)(destIndex));
        mergeWord = mergeFnwith(skewWord & halftoneWord, destWord & destMask);
        destWord =
            (destMask & mergeWord) | (destWord & ((unsigned int)~destMask));

        /* dstLongAt:put: */
        long32Atput((void *)(destIndex), destWord);
      }

      /* avoid read-modify-write
         General version using dest masking */

      /* incDestIndex: */
      destIndex += 4;
      if (words == 2) {
        destMask = mask2;
        nPix = endBits;
      } else {
        destMask = AllOnes;
        nPix = destPPW;
      }

      /* e.g., is the next word the last word?
         set mask for last word in this row
         use fullword mask for inner loop */
    } while (!(((words -= 1)) == 0));

    /* --- end of inner loop --- */

    /* incSrcIndex: */
    sourceIndex += sourceDelta;

    /* incDestIndex: */
    destIndex += destDelta;
  }
  return 0;
}