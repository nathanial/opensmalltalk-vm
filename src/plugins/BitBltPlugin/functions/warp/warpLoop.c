/* Extracted from BitBltPlugin.c:6166 (function warpLoop). */
/* BitBltSimulation>>#warpLoop */
/*	Pick n (sub-) pixels from the source form, mapped by sourceMap,
        average the RGB values, map by colorMap and return the new word.
        This version is only called from WarpBlt with smoothingCount > 1 */

static sqInt warpLoop(void) {
  sqInt deltaP12x;
  sqInt deltaP12y;
  sqInt deltaP43x;
  sqInt deltaP43y;
  sqInt destPix;
  unsigned int destWord;
  sqInt destWordSqInt;
  int dstMask;
  int dstShiftInc;
  int dstShiftLeft;
  sqInt endBits;
  sqInt fieldOop;
  double floatValue;
  unsigned int halftoneWord;
  sqInt i;
  sqInt iSqInt;
  sqInt mapperFlags;
  unsigned int (*mergeFnwith)(unsigned int, unsigned int);
  unsigned int mergeWord;
  sqInt nPix;
  sqInt nPixSqInt;
  sqInt nSteps;
  sqInt pAx;
  sqInt pAy;
  sqInt pBx;
  sqInt pBy;
  unsigned int skewWord;
  sqInt smoothingCount;
  sqInt sourceMapOop;
  unsigned int sourcePix;
  sqInt sourcePixSqInt;
  unsigned int sourceWord;
  sqInt srcIndex;
  sqInt startBits;
  sqInt val;
  sqInt words;
  sqInt wordsSqInt;
  sqInt x;
  sqInt xDelta;
  sqInt y;
  sqInt yDelta;

  halftoneWord = 0;
  mergeFnwith = ((unsigned int (*)(unsigned int, unsigned int))(
      opTable[combinationRule + 1]));
  if (!((slotSizeOf(bitBltOop)) >= (BBWarpBase + 12))) {
    return primitiveFail();
  }
  nSteps = height - 1;
  if (nSteps <= 0) {
    nSteps = 1;
  }

  /* begin fetchIntOrFloat:ofObject: */
  fieldOop = fetchPointerofObject(BBWarpBase, bitBltOop);
  if (isIntegerObject(fieldOop)) {
    pAx = integerValueOf(fieldOop);
    goto l1;
  }
  floatValue = floatValueOf(fieldOop);
  if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
    primitiveFail();
    pAx = 0;
    goto l1;
  }
  pAx = ((sqInt)floatValue);
  /* end fetchIntOrFloat:ofObject: */
l1:

  /* begin fetchIntOrFloat:ofObject: */
  fieldOop = fetchPointerofObject(BBWarpBase + 3, bitBltOop);
  if (isIntegerObject(fieldOop)) {
    words = integerValueOf(fieldOop);
    goto l2;
  }
  floatValue = floatValueOf(fieldOop);
  if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
    primitiveFail();
    words = 0;
    goto l2;
  }
  words = ((sqInt)floatValue);
  /* end fetchIntOrFloat:ofObject: */
l2:

  /* begin deltaFrom:to:nSteps: */
  if (words > pAx) {
    deltaP12x = (((words - pAx) + FixedPt1) / (nSteps + 1)) + 1;
  } else {
    if (words == pAx) {
      deltaP12x = 0;
      goto l3;
    }
    deltaP12x = 0 - ((((pAx - words) + FixedPt1) / (nSteps + 1)) + 1);
  }
  /* end deltaFrom:to:nSteps: */
l3:
  if (deltaP12x < 0) {
    pAx = words - (nSteps * deltaP12x);
  }

  /* begin fetchIntOrFloat:ofObject: */
  fieldOop = fetchPointerofObject(BBWarpBase + 1, bitBltOop);
  if (isIntegerObject(fieldOop)) {
    pAy = integerValueOf(fieldOop);
    goto l4;
  }
  floatValue = floatValueOf(fieldOop);
  if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
    primitiveFail();
    pAy = 0;
    goto l4;
  }
  pAy = ((sqInt)floatValue);
  /* end fetchIntOrFloat:ofObject: */
l4:

  /* begin fetchIntOrFloat:ofObject: */
  fieldOop = fetchPointerofObject(BBWarpBase + 4, bitBltOop);
  if (isIntegerObject(fieldOop)) {
    words = integerValueOf(fieldOop);
    goto l5;
  }
  floatValue = floatValueOf(fieldOop);
  if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
    primitiveFail();
    words = 0;
    goto l5;
  }
  words = ((sqInt)floatValue);
  /* end fetchIntOrFloat:ofObject: */
l5:

  /* begin deltaFrom:to:nSteps: */
  if (words > pAy) {
    deltaP12y = (((words - pAy) + FixedPt1) / (nSteps + 1)) + 1;
  } else {
    if (words == pAy) {
      deltaP12y = 0;
      goto l6;
    }
    deltaP12y = 0 - ((((pAy - words) + FixedPt1) / (nSteps + 1)) + 1);
  }
  /* end deltaFrom:to:nSteps: */
l6:
  if (deltaP12y < 0) {
    pAy = words - (nSteps * deltaP12y);
  }

  /* begin fetchIntOrFloat:ofObject: */
  fieldOop = fetchPointerofObject(BBWarpBase + 9, bitBltOop);
  if (isIntegerObject(fieldOop)) {
    pBx = integerValueOf(fieldOop);
    goto l7;
  }
  floatValue = floatValueOf(fieldOop);
  if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
    primitiveFail();
    pBx = 0;
    goto l7;
  }
  pBx = ((sqInt)floatValue);
  /* end fetchIntOrFloat:ofObject: */
l7:

  /* begin fetchIntOrFloat:ofObject: */
  fieldOop = fetchPointerofObject(BBWarpBase + 6, bitBltOop);
  if (isIntegerObject(fieldOop)) {
    words = integerValueOf(fieldOop);
    goto l8;
  }
  floatValue = floatValueOf(fieldOop);
  if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
    primitiveFail();
    words = 0;
    goto l8;
  }
  words = ((sqInt)floatValue);
  /* end fetchIntOrFloat:ofObject: */
l8:

  /* begin deltaFrom:to:nSteps: */
  if (words > pBx) {
    deltaP43x = (((words - pBx) + FixedPt1) / (nSteps + 1)) + 1;
  } else {
    if (words == pBx) {
      deltaP43x = 0;
      goto l9;
    }
    deltaP43x = 0 - ((((pBx - words) + FixedPt1) / (nSteps + 1)) + 1);
  }
  /* end deltaFrom:to:nSteps: */
l9:
  if (deltaP43x < 0) {
    pBx = words - (nSteps * deltaP43x);
  }

  /* begin fetchIntOrFloat:ofObject: */
  fieldOop = fetchPointerofObject(BBWarpBase + 10, bitBltOop);
  if (isIntegerObject(fieldOop)) {
    pBy = integerValueOf(fieldOop);
    goto l10;
  }
  floatValue = floatValueOf(fieldOop);
  if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
    primitiveFail();
    pBy = 0;
    goto l10;
  }
  pBy = ((sqInt)floatValue);
  /* end fetchIntOrFloat:ofObject: */
l10:

  /* begin fetchIntOrFloat:ofObject: */
  fieldOop = fetchPointerofObject(BBWarpBase + 7, bitBltOop);
  if (isIntegerObject(fieldOop)) {
    words = integerValueOf(fieldOop);
    goto l11;
  }
  floatValue = floatValueOf(fieldOop);
  if (!((-2.147483648e9 <= floatValue) && (floatValue <= 2.147483647e9))) {
    primitiveFail();
    words = 0;
    goto l11;
  }
  words = ((sqInt)floatValue);
  /* end fetchIntOrFloat:ofObject: */
l11:

  /* begin deltaFrom:to:nSteps: */
  if (words > pBy) {
    deltaP43y = (((words - pBy) + FixedPt1) / (nSteps + 1)) + 1;
  } else {
    if (words == pBy) {
      deltaP43y = 0;
      goto l12;
    }
    deltaP43y = 0 - ((((pBy - words) + FixedPt1) / (nSteps + 1)) + 1);
  }
  /* end deltaFrom:to:nSteps: */
l12:
  if (deltaP43y < 0) {
    pBy = words - (nSteps * deltaP43y);
  }
  if (failed()) {
    return 0;
  }
  if ((methodArgumentCount()) == 2) {
    smoothingCount = stackIntegerValue(1);
    sourceMapOop = stackValue(0);
    if (sourceMapOop == (nilObject())) {
      if (sourceDepth < 16) {
        return primitiveFail();
      }
    } else {
      if ((slotSizeOf(sourceMapOop)) < (1U << sourceDepth)) {
        return primitiveFail();
      }

      /* sourceMap must be long enough for sourceDepth */
      sourceMapOop = oopForPointer(firstIndexableField(sourceMapOop));
    }
  } else {
    smoothingCount = 1;
    sourceMapOop = nilObject();
  }
  nSteps = width - 1;
  if (nSteps <= 0) {
    nSteps = 1;
  }
  startBits = destPPW - (dx & (destPPW - 1));
  endBits = (((dx + bbW) - 1) & (destPPW - 1)) + 1;
  if (bbW < startBits) {
    startBits = bbW;
  }
  if (destY < clipY) {
    pAx += (clipY - destY) * deltaP12x;
    pAy += (clipY - destY) * deltaP12y;
    pBx += (clipY - destY) * deltaP43x;
    pBy += (clipY - destY) * deltaP43y;
  }

  /* Advance increments if there was clipping in y
     Setup values for faster pixel fetching. */

  /* begin warpLoopSetup */
  warpSrcShift = 0;

  /* recycle temp */
  wordsSqInt = sourceDepth;
  while (!(wordsSqInt == 1)) {
    warpSrcShift += 1;
    wordsSqInt = ((usqInt)(wordsSqInt)) >> 1;
  }

  /* warpSrcMask = mask for extracting one pixel from source word */
  warpSrcMask = maskTable[sourceDepth];

  /* warpAlignShift: Shift for aligning x position to word boundary */
  warpAlignShift = 5 - warpSrcShift;

  /* warpAlignMask: Mask for extracting the pixel position from an x position */
  warpAlignMask = (1U << warpAlignShift) - 1;

  /* Setup the lookup table for source bit shifts
     warpBitShiftTable: given an sub-word x value what's the bit shift? */
  for (iSqInt = 0; iSqInt <= warpAlignMask; iSqInt += 1) {
    if (sourceMSB) {
      warpBitShiftTable[iSqInt] =
          (32 - ((((usqInt)((iSqInt + 1)) << warpSrcShift))));
    } else {
      warpBitShiftTable[iSqInt] = (((sqInt)((usqInt)(iSqInt) << warpSrcShift)));
    }
  }

  /* Setup color mapping if not provided */
  if ((smoothingCount > 1) && ((cmFlags & ColorMapNewStyle) == 0)) {
    if (cmLookupTable) {
      setupColorMasksFromto(8, cmBitsPerColor);
    } else {
      if (destDepth == 16) {
        setupColorMasksFromto(8, 5);
      }
    }
  }
  mapperFlags = cmFlags & ((unsigned int)~ColorMapNewStyle);
  if (destMSB) {
    dstShiftInc = 0 - destDepth;
    dstShiftLeft = 32 - destDepth;
  } else {
    dstShiftInc = destDepth;
    dstShiftLeft = 0;
  }
  if (noHalftone) {
    halftoneWord = AllOnes;
  }
  for (i = 1; i <= bbH; i += 1) {
    /* begin deltaFrom:to:nSteps: */
    if (pBx > pAx) {
      xDelta = (((pBx - pAx) + FixedPt1) / (nSteps + 1)) + 1;
    } else {
      if (pBx == pAx) {
        xDelta = 0;
        goto l13;
      }
      xDelta = 0 - ((((pAx - pBx) + FixedPt1) / (nSteps + 1)) + 1);
    }
    /* end deltaFrom:to:nSteps: */
  l13:
    if (xDelta >= 0) {
      sx = pAx;
    } else {
      sx = pBx - (nSteps * xDelta);
    }

    /* begin deltaFrom:to:nSteps: */
    if (pBy > pAy) {
      yDelta = (((pBy - pAy) + FixedPt1) / (nSteps + 1)) + 1;
    } else {
      if (pBy == pAy) {
        yDelta = 0;
        goto l14;
      }
      yDelta = 0 - ((((pAy - pBy) + FixedPt1) / (nSteps + 1)) + 1);
    }
    /* end deltaFrom:to:nSteps: */
  l14:
    if (yDelta >= 0) {
      sy = pAy;
    } else {
      sy = pBy - (nSteps * yDelta);
    }
    if (destMSB) {
      dstBitShift = 32 - (((dx & (destPPW - 1)) + 1) * destDepth);
    } else {
      dstBitShift = (dx & (destPPW - 1)) * destDepth;
    }
    if (destX < clipX) {
      sx += (clipX - destX) * xDelta;
      sy += (clipX - destX) * yDelta;
    }

    /* Advance increments if there was clipping in x */
    if (!noHalftone) {
      halftoneWord = long32At(
          (void *)(halftoneBase + ((((dy + i) - 1) % halftoneHeight) * 4)));
    }
    destMask = mask1;
    nPix = startBits;

    /* Here is the inner loop... */
    words = nWords;

    /* pick up word */
    do {
      if (smoothingCount == 1) {
        /* begin
         * warpPickSourcePixels:xDeltah:yDeltah:xDeltav:yDeltav:dstShiftInc:flags:
         */
        dstMask = maskTable[destDepth];
        destWordSqInt = 0;
        nPixSqInt = nPix;
        if (mapperFlags == (ColorMapPresent | ColorMapIndexedPart)) {
          do {
            /* begin pickWarpPixelAtX:y: */
            if ((sx < 0) ||
                ((sy < 0) ||
                 ((((x = ((usqInt)(sx)) >> BinaryPoint)) >= sourceWidth) ||
                  (((y = ((usqInt)(sy)) >> BinaryPoint)) >= sourceHeight)))) {
              sourcePix = 0;
              goto l15;
            }
            srcIndex = (sourceBits + (y * sourcePitch)) +
                       ((((usqInt)(x)) >> warpAlignShift) * 4);

            /* begin srcLongAt: */
            assert((((usqInt)srcIndex)) < endOfSource);
            sourceWord = long32At((void *)(srcIndex));

            /* Extract pixel from word */
            srcBitShift = warpBitShiftTable[x & warpAlignMask];
            sourcePixSqInt =
                (((usqInt)(sourceWord)) >> srcBitShift) & warpSrcMask;
            sourcePix = ((unsigned int)sourcePixSqInt);
            /* end pickWarpPixelAtX:y: */
          l15:
            destPix = cmLookupTable[sourcePix & cmMask];
            destWordSqInt = destWordSqInt |
                            ((((usqInt)((destPix & dstMask)) << dstBitShift)));
            dstBitShift += dstShiftInc;
            sx += xDelta;
            sy += yDelta;
          } while (!(((nPixSqInt -= 1)) == 0));
        } else {
          do {
            /* begin pickWarpPixelAtX:y: */
            if ((sx < 0) ||
                ((sy < 0) ||
                 ((((x = ((usqInt)(sx)) >> BinaryPoint)) >= sourceWidth) ||
                  (((y = ((usqInt)(sy)) >> BinaryPoint)) >= sourceHeight)))) {
              sourcePix = 0;
              goto l16;
            }
            srcIndex = (sourceBits + (y * sourcePitch)) +
                       ((((usqInt)(x)) >> warpAlignShift) * 4);

            /* begin srcLongAt: */
            assert((((usqInt)srcIndex)) < endOfSource);
            sourceWord = long32At((void *)(srcIndex));

            /* Extract pixel from word */
            srcBitShift = warpBitShiftTable[x & warpAlignMask];
            sourcePixSqInt =
                (((usqInt)(sourceWord)) >> srcBitShift) & warpSrcMask;
            sourcePix = ((unsigned int)sourcePixSqInt);
            /* end pickWarpPixelAtX:y: */
          l16:

            /* begin mapPixel:flags: */
            destPix = sourcePix;
            if (mapperFlags & ColorMapPresent) {
              if (mapperFlags & ColorMapFixedPart) {
                /* begin rgbMapPixel:flags: */
                val = (((((int)(cmShiftTable[0]))) < 0)
                           ? (((usqInt)((sourcePix & (cmMaskTable[0])))) >>
                              (-(((int)(cmShiftTable[0])))))
                           : ((((usqInt)((sourcePix & (cmMaskTable[0])))
                                << (((int)(cmShiftTable[0])))))));
                val =
                    val | ((((((int)(cmShiftTable[1]))) < 0)
                                ? (((usqInt)((sourcePix & (cmMaskTable[1])))) >>
                                   (-(((int)(cmShiftTable[1])))))
                                : ((((usqInt)((sourcePix & (cmMaskTable[1])))
                                     << (((int)(cmShiftTable[1]))))))));
                val =
                    val | ((((((int)(cmShiftTable[2]))) < 0)
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
            destWordSqInt = destWordSqInt |
                            ((((usqInt)((destPix & dstMask)) << dstBitShift)));
            dstBitShift += dstShiftInc;
            sx += xDelta;
            sy += yDelta;
          } while (!(((nPixSqInt -= 1)) == 0));
        }

        /* a little optimization for (pretty crucial) blits using indexed
           lookups only grab, colormap and mix in pixel grab, colormap and mix
           in pixel */
        skewWord = destWordSqInt;
      } else {
        skewWord =
            warpPickSmoothPixelsxDeltahyDeltahxDeltavyDeltavsourceMapsmoothingdstShiftInc(
                nPix, xDelta, yDelta, deltaP12x, deltaP12y, sourceMapOop,
                smoothingCount, dstShiftInc);
      }

      /* Faster if not smoothing
         more difficult with smoothing
         align next word access to left most pixel */
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
    pAx += deltaP12x;
    pAy += deltaP12y;
    pBx += deltaP43x;
    pBy += deltaP43y;

    /* incDestIndex: */
    destIndex += destDelta;
  }
  return 0;
}
