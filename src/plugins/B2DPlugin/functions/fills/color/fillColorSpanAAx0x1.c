/* Extracted from B2DPlugin.c:2087 (function fillColorSpanAAx0x1). */

static sqInt fillColorSpanAAx0x1(sqInt pixelValue32, sqInt leftX,
                                 sqInt rightX) {
  int aaLevel;
  int baseShift;
  unsigned int colorMask;
  sqInt firstPixel;
  sqInt firstPixelSqInt;
  sqInt idx;
  sqInt lastPixel;
  sqInt pv32;
  sqInt x;

  /* Not now -- maybe later */
  /* Compute the pixel boundaries. */
  /* begin aaFirstPixelFrom:to: */
  firstPixelSqInt = ((leftX + (workBuffer[GWAALevel])) - 1) &
                    ((unsigned int)~((workBuffer[GWAALevel]) - 1));
  if (firstPixelSqInt > rightX) {
    firstPixel = rightX;
  } else {
    firstPixel = firstPixelSqInt;
  }
  lastPixel = (rightX - 1) & ((unsigned int)~((workBuffer[GWAALevel]) - 1));
  aaLevel = workBuffer[GWAALevel];
  baseShift = workBuffer[GWAAShift];
  x = leftX;

  /* Part a: Deal with the first n sub-pixels */
  if (x < firstPixel) {
    pv32 = ((usqInt)((pixelValue32 & (workBuffer[GWAAColorMask])))) >>
           (workBuffer[GWAAColorShift]);
    while (x < firstPixel) {
      idx = ((usqInt)(x)) >> baseShift;
      spanBuffer[idx] = ((spanBuffer[idx]) + pv32);
      x += 1;
    }
  }

  /* Part b: Deal with the full pixels */
  if (x < lastPixel) {
    colorMask =
        (((usqInt)((workBuffer[GWAAColorMask]))) >> (workBuffer[GWAAShift])) |
        0xF0F0F0F0U;
    pv32 = ((usqInt)((pixelValue32 & colorMask))) >> (workBuffer[GWAAShift]);
    while (x < lastPixel) {
      idx = ((usqInt)(x)) >> baseShift;
      spanBuffer[idx] = ((spanBuffer[idx]) + pv32);
      x += aaLevel;
    }
  }

  /* Part c: Deal with the last n sub-pixels */
  if (x < rightX) {
    pv32 = ((usqInt)((pixelValue32 & (workBuffer[GWAAColorMask])))) >>
           (workBuffer[GWAAColorShift]);
    while (x < rightX) {
      idx = ((usqInt)(x)) >> baseShift;
      spanBuffer[idx] = ((spanBuffer[idx]) + pv32);
      x += 1;
    }
  }
  return 0;
}