/* Extracted from B2DPlugin.c:2154 (function fillLinearGradientAArampdsdsXfromto). */

static sqInt fillLinearGradientAArampdsdsXfromto(sqInt fill, int *ramp,
                                                 sqInt deltaS, sqInt dsX,
                                                 sqInt leftX, sqInt rightX) {
  int aaLevel;
  int baseShift;
  unsigned int colorMask;
  int colorShift;
  sqInt ds;
  sqInt firstPixel;
  sqInt firstPixelSqInt;
  sqInt idx;
  sqInt lastPixel;
  sqInt rampIndex;
  int rampSize;
  sqInt rampValue;
  sqInt x;

  aaLevel = workBuffer[GWAALevel];
  baseShift = workBuffer[GWAAShift];
  rampSize = objBuffer[fill + GFRampLength];
  ds = deltaS;
  x = leftX;
  rampIndex = ds / 0x10000;

  /* begin aaFirstPixelFrom:to: */
  firstPixelSqInt = ((leftX + (workBuffer[GWAALevel])) - 1) &
                    ((unsigned int)~((workBuffer[GWAALevel]) - 1));
  if (firstPixelSqInt > rightX) {
    firstPixel = rightX;
  } else {
    firstPixel = firstPixelSqInt;
  }
  lastPixel = (rightX - 1) & ((unsigned int)~((workBuffer[GWAALevel]) - 1));

  /* Deal with the first n sub-pixels */
  colorMask = workBuffer[GWAAColorMask];
  colorShift = workBuffer[GWAAColorShift];
  while ((x < firstPixel) && ((rampIndex < rampSize) && (rampIndex >= 0))) {
    rampValue = (((int *)ramp))[rampIndex];
    rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;

    /* Copy as many pixels as possible */
    while ((x < firstPixel) && ((ds / 0x10000) == rampIndex)) {
      idx = ((usqInt)(x)) >> baseShift;
      spanBuffer[idx] = ((spanBuffer[idx]) + rampValue);
      x += 1;
      ds += dsX;
    }
    rampIndex = ds / 0x10000;
  }

  /* Deal with the full pixels */
  colorMask =
      (((usqInt)((workBuffer[GWAAColorMask]))) >> (workBuffer[GWAAShift])) |
      0xF0F0F0F0U;
  colorShift = workBuffer[GWAAShift];
  while ((x < lastPixel) && ((rampIndex < rampSize) && (rampIndex >= 0))) {
    rampValue = (((int *)ramp))[rampIndex];
    rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;

    /* Copy as many pixels as possible */
    while ((x < lastPixel) && ((ds / 0x10000) == rampIndex)) {
      idx = ((usqInt)(x)) >> baseShift;
      spanBuffer[idx] = ((spanBuffer[idx]) + rampValue);
      x += aaLevel;
      ds += (((usqInt)(dsX) << colorShift));
    }
    rampIndex = ds / 0x10000;
  }

  /* Deal with the last n sub-pixels */
  colorMask = workBuffer[GWAAColorMask];
  colorShift = workBuffer[GWAAColorShift];
  while ((x < rightX) && ((rampIndex < rampSize) && (rampIndex >= 0))) {
    rampValue = (((int *)ramp))[rampIndex];
    rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;

    /* Copy as many pixels as possible */
    while ((x < rightX) && ((ds / 0x10000) == rampIndex)) {
      idx = ((usqInt)(x)) >> baseShift;
      spanBuffer[idx] = ((spanBuffer[idx]) + rampValue);
      x += 1;
      ds += dsX;
    }
    rampIndex = ds / 0x10000;
  }
  return x;
}