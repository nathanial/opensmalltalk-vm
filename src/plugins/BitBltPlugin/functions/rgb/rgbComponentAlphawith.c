/* Extracted from BitBltPlugin.c:5441 (function rgbComponentAlphawith). */
/* BitBltSimulation>>#rgbComponentAlpha:with: */
/*	Subract the pixels in the source and destination, color by color,
        and return the sum of the absolute value of all the differences.
        For non-rgb, return the number of differing pixels. */

static sqInt rgbComponentAlphawith(sqInt sourceWord, sqInt destinationWord) {
  sqInt alpha;
  sqInt d;
  sqInt destPix;
  unsigned int destWord;
  sqInt i;
  unsigned int mask;
  sqInt maskSqInt;
  unsigned int p1;
  unsigned int p2;
  unsigned int result;
  unsigned int sourceWord1;
  sqInt srcPix;
  sqInt v;

  alpha = sourceWord;

  /* This is not a valid optimisation because alpha == 0 can change the
     destination due to rounding errors in blending and/or in gamma lookup
     round-trip alpha = 0 ifTrue:[^destinationWord]. */
  sourceWord1 = ((unsigned int)sourceWord);
  destWord = ((unsigned int)destinationWord);

  /* begin partitionedRgbComponentAlpha:dest:nBits:nPartitions: */
  /* partition mask starts at the right */
  mask = maskTable[destDepth];
  result = 0;
  for (i = 1; i <= destPPW; i += 1) {
    p1 = ((usqInt)((sourceWord1 & mask))) >> ((i - 1) * destDepth);
    p2 = ((usqInt)((destWord & mask))) >> ((i - 1) * destDepth);
    if (!(destDepth == 32)) {
      if (destDepth == 16) {
        p1 = ((((((usqInt)((p1 & 0x1F)) << 3))) |
               ((((usqInt)((p1 & 0x3E0)) << 6)))) |
              ((((usqInt)((p1 & 0x7C00)) << 9)))) |
             0xFF000000U;
        p2 = ((((((usqInt)((p2 & 0x1F)) << 3))) |
               ((((usqInt)((p2 & 0x3E0)) << 6)))) |
              ((((usqInt)((p2 & 0x7C00)) << 9)))) |
             0xFF000000U;
      } else {
        p1 = (rgbMapfromto(p1, destDepth, 32)) | 0xFF000000U;
        p2 = (rgbMapfromto(p2, destDepth, 32)) | 0xFF000000U;
      }
    }
    v = rgbComponentAlpha32with(p1, p2);
    if (!(destDepth == 32)) {
      /* begin rgbMap:from:to: */
      if (((d = destDepth - 32)) > 0) {
        /* Transfer mask */
        maskSqInt = 0xFFFFFFFFU;
        srcPix = ((sqInt)((usqInt)(v) << d));
        maskSqInt = ((sqInt)((usqInt)(maskSqInt) << d));
        destPix = srcPix & maskSqInt;
        maskSqInt = ((sqInt)((usqInt)(maskSqInt) << destDepth));
        srcPix = ((sqInt)((usqInt)(srcPix) << d));
        v = (destPix + (srcPix & maskSqInt)) +
            (((((usqInt)(srcPix) << d))) &
             ((((usqInt)(maskSqInt) << destDepth))));
      } else {
        if (!d) {

          /* Sometimes called with 16 bits, though pixel is 15,
             but we must never return more than 15. */

          /* Sometimes called with 32 bits, though pixel is 24,
             but we must never return more than 24. */
          goto l1;
        }
        if (!v) {
          goto l1;
        }
        d = 32 - destDepth;

        /* Transfer mask */
        maskSqInt = (1U << destDepth) - 1;
        srcPix = ((usqInt)(v)) >> d;
        destPix = srcPix & maskSqInt;
        maskSqInt = ((sqInt)((usqInt)(maskSqInt) << destDepth));
        srcPix = ((usqInt)(srcPix)) >> d;
        destPix = (destPix + (srcPix & maskSqInt)) +
                  ((((usqInt)(srcPix)) >> d) &
                   ((((usqInt)(maskSqInt) << destDepth))));
        if (!destPix) {
          v = 1;
          goto l1;
        }
        v = destPix;
      }
      /* end rgbMap:from:to: */
    l1:;
    }
    result = result | ((((usqInt)(v) << ((i - 1) * destDepth))));

    /* slide left to next partition */
    mask = (((usqInt)(mask) << destDepth));
  }
  return result;
}
