/* Extracted from B2DPlugin.c:2688 (function fillRadialIncreasingAArampdeltaSTdsXdtXfromto). */

static sqInt fillRadialIncreasingAArampdeltaSTdsXdtXfromto(
    sqInt fill, int *ramp, int *deltaST, sqInt dsX, sqInt dtX, sqInt leftX,
    sqInt rightX) {
  int aaLevel;
  int baseShift;
  unsigned int colorMask;
  int colorShift;
  int ds;
  int dt;
  sqInt firstPixel;
  sqInt firstPixelSqInt;
  sqInt index;
  sqInt lastLength;
  sqInt lastPixel;
  int length2;
  sqInt nextLength;
  sqInt rampIndex;
  int rampSize;
  sqInt rampValue;
  sqInt x;

  ds = (*((int *)deltaST));
  dt = (((int *)deltaST))[1];
  aaLevel = workBuffer[GWAALevel];
  baseShift = workBuffer[GWAAShift];
  rampIndex = accurateLengthOfwith(ds / 0x10000, dt / 0x10000);
  rampSize = objBuffer[fill + GFRampLength];

  /* This is the upper bound */
  length2 = (rampSize - 1) * (rampSize - 1);
  nextLength = (rampIndex + 1) * (rampIndex + 1);
  lastLength =
      ((ds / 0x10000) * (ds / 0x10000)) + ((dt / 0x10000) * (dt / 0x10000));
  x = leftX;

  /* begin aaFirstPixelFrom:to: */
  firstPixelSqInt = ((leftX + (workBuffer[GWAALevel])) - 1) &
                    ((unsigned int)~((workBuffer[GWAALevel]) - 1));
  if (firstPixelSqInt > rightX) {
    firstPixel = rightX;
  } else {
    firstPixel = firstPixelSqInt;
  }
  lastPixel = (rightX - 1) & ((unsigned int)~((workBuffer[GWAALevel]) - 1));

  /* Deal with the first n subPixels */
  if ((x < firstPixel) && (lastLength < length2)) {
    colorMask = workBuffer[GWAAColorMask];
    colorShift = workBuffer[GWAAColorShift];
    rampValue = (((int *)ramp))[rampIndex];
    rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
    while ((x < firstPixel) && (lastLength < length2)) {
      while ((x < firstPixel) &&
             ((((ds / 0x10000) * (ds / 0x10000)) +
               ((dt / 0x10000) * (dt / 0x10000))) <= nextLength)) {
        index = ((usqInt)(x)) >> baseShift;
        spanBuffer[index] = ((spanBuffer[index]) + rampValue);
        x += 1;
        ds += dsX;
        dt += dtX;
      }
      lastLength =
          ((ds / 0x10000) * (ds / 0x10000)) + ((dt / 0x10000) * (dt / 0x10000));
      while (lastLength > nextLength) {
        rampIndex += 1;
        rampValue = (((int *)ramp))[rampIndex];
        rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
        nextLength = (rampIndex + 1) * (rampIndex + 1);
      }
    }
  }

  /* Deal with the full pixels */
  if ((x < lastPixel) && (lastLength < length2)) {
    colorMask =
        (((usqInt)((workBuffer[GWAAColorMask]))) >> (workBuffer[GWAAShift])) |
        0xF0F0F0F0U;
    colorShift = workBuffer[GWAAShift];
    rampValue = (((int *)ramp))[rampIndex];
    rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
    while ((x < lastPixel) && (lastLength < length2)) {
      while ((x < lastPixel) &&
             ((((ds / 0x10000) * (ds / 0x10000)) +
               ((dt / 0x10000) * (dt / 0x10000))) <= nextLength)) {
        index = ((usqInt)(x)) >> baseShift;
        spanBuffer[index] = ((spanBuffer[index]) + rampValue);
        x += aaLevel;
        ds += (((usqInt)(dsX) << colorShift));
        dt += (((usqInt)(dtX) << colorShift));
      }
      lastLength =
          ((ds / 0x10000) * (ds / 0x10000)) + ((dt / 0x10000) * (dt / 0x10000));
      while (lastLength > nextLength) {
        rampIndex += 1;
        rampValue = (((int *)ramp))[rampIndex];
        rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
        nextLength = (rampIndex + 1) * (rampIndex + 1);
      }
    }
  }

  /* Deal with last n sub-pixels */
  if ((x < rightX) && (lastLength < length2)) {
    colorMask = workBuffer[GWAAColorMask];
    colorShift = workBuffer[GWAAColorShift];
    rampValue = (((int *)ramp))[rampIndex];
    rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
    while ((x < rightX) && (lastLength < length2)) {
      while ((x < rightX) &&
             ((((ds / 0x10000) * (ds / 0x10000)) +
               ((dt / 0x10000) * (dt / 0x10000))) <= nextLength)) {
        index = ((usqInt)(x)) >> baseShift;
        spanBuffer[index] = ((spanBuffer[index]) + rampValue);
        x += 1;
        ds += dsX;
        dt += dtX;
      }
      lastLength =
          ((ds / 0x10000) * (ds / 0x10000)) + ((dt / 0x10000) * (dt / 0x10000));
      while (lastLength > nextLength) {
        rampIndex += 1;
        rampValue = (((int *)ramp))[rampIndex];
        rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
        nextLength = (rampIndex + 1) * (rampIndex + 1);
      }
    }
  }

  /* Done -- store stuff back */
  (*((int *)deltaST) = ds);
  (((int *)deltaST))[1] = dt;
  return x;
}