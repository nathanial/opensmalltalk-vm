/* Extracted from BitBltPlugin.c:5606 (function rgbMapfromto). */

static sqInt rgbMapfromto(sqInt sourcePixel, sqInt nBitsIn, sqInt nBitsOut) {
  sqInt d;
  sqInt destPix;
  sqInt mask;
  sqInt srcPix;

  if (((d = nBitsOut - nBitsIn)) > 0) {
    /* Transfer mask */
    mask = (1U << nBitsIn) - 1;
    srcPix = ((sqInt)((usqInt)(sourcePixel) << d));
    mask = ((sqInt)((usqInt)(mask) << d));
    destPix = srcPix & mask;
    mask = ((sqInt)((usqInt)(mask) << nBitsOut));
    srcPix = ((sqInt)((usqInt)(srcPix) << d));
    return (destPix + (srcPix & mask)) +
           (((((usqInt)(srcPix) << d))) & ((((usqInt)(mask) << nBitsOut))));
  } else {
    if (!d) {
      if (nBitsIn == 5) {
        return sourcePixel & 0x7FFF;
      }

      /* Sometimes called with 16 bits, though pixel is 15,
         but we must never return more than 15. */
      if (nBitsIn == 8) {
        return sourcePixel & 0xFFFFFF;
      }

      /* Sometimes called with 32 bits, though pixel is 24,
         but we must never return more than 24. */
      return sourcePixel;
    }
    if (!sourcePixel) {
      return sourcePixel;
    }
    d = nBitsIn - nBitsOut;

    /* Transfer mask */
    mask = (1U << nBitsOut) - 1;
    srcPix = ((usqInt)(sourcePixel)) >> d;
    destPix = srcPix & mask;
    mask = ((sqInt)((usqInt)(mask) << nBitsOut));
    srcPix = ((usqInt)(srcPix)) >> d;
    destPix = (destPix + (srcPix & mask)) +
              ((((usqInt)(srcPix)) >> d) & ((((usqInt)(mask) << nBitsOut))));
    if (!destPix) {
      return 1;
    }
    return destPix;
  }
}