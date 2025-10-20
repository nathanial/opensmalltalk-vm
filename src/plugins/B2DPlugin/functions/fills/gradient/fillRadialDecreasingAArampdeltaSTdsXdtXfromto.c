/* Extracted from B2DPlugin.c:2354 (function fillRadialDecreasingAArampdeltaSTdsXdtXfromto). */

static sqInt fillRadialDecreasingAArampdeltaSTdsXdtXfromto(
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
  sqInt lastPixel;
  sqInt length2;
  sqInt nextLength;
  sqInt rampIndex;
  sqInt rampValue;
  sqInt x;
  sqInt x1;

  ds = (*((int *)deltaST));
  dt = (((int *)deltaST))[1];
  aaLevel = workBuffer[GWAALevel];
  baseShift = workBuffer[GWAAShift];
  rampIndex = accurateLengthOfwith(ds / 0x10000, dt / 0x10000);
  length2 = (rampIndex - 1) * (rampIndex - 1);
  x = leftX;
  x1 = objBuffer[fill + GFOriginX];
  if (x1 > rightX) {
    x1 = rightX;
  }

  /* begin aaFirstPixelFrom:to: */
  firstPixelSqInt = ((leftX + (workBuffer[GWAALevel])) - 1) &
                    ((unsigned int)~((workBuffer[GWAALevel]) - 1));
  if (firstPixelSqInt > x1) {
    firstPixel = x1;
  } else {
    firstPixel = firstPixelSqInt;
  }
  lastPixel = (x1 - 1) & ((unsigned int)~((workBuffer[GWAALevel]) - 1));

  /* Deal with the first n sub-pixels */
  if (x < firstPixel) {
    colorMask = workBuffer[GWAAColorMask];
    colorShift = workBuffer[GWAAColorShift];
    rampValue = (((int *)ramp))[rampIndex];
    rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
    while (x < firstPixel) {
      while ((x < firstPixel) &&
             ((((ds / 0x10000) * (ds / 0x10000)) +
               ((dt / 0x10000) * (dt / 0x10000))) >= length2)) {
        index = ((usqInt)(x)) >> baseShift;
        spanBuffer[index] = ((spanBuffer[index]) + rampValue);
        x += 1;
        ds += dsX;
        dt += dtX;
      }

      /* Step to next ramp value */
      nextLength =
          ((ds / 0x10000) * (ds / 0x10000)) + ((dt / 0x10000) * (dt / 0x10000));
      while (nextLength < length2) {
        rampIndex -= 1;
        rampValue = (((int *)ramp))[rampIndex];
        rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
        length2 = (rampIndex - 1) * (rampIndex - 1);
      }
    }
  }

  /* Deal with the full pixels */
  if (x < lastPixel) {
    colorMask =
        (((usqInt)((workBuffer[GWAAColorMask]))) >> (workBuffer[GWAAShift])) |
        0xF0F0F0F0U;
    colorShift = workBuffer[GWAAShift];
    rampValue = (((int *)ramp))[rampIndex];
    rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
    while (x < lastPixel) {
      while ((x < lastPixel) &&
             ((((ds / 0x10000) * (ds / 0x10000)) +
               ((dt / 0x10000) * (dt / 0x10000))) >= length2)) {
        index = ((usqInt)(x)) >> baseShift;
        spanBuffer[index] = ((spanBuffer[index]) + rampValue);
        x += aaLevel;
        ds += (((usqInt)(dsX) << colorShift));
        dt += (((usqInt)(dtX) << colorShift));
      }

      /* Step to next ramp value */
      nextLength =
          ((ds / 0x10000) * (ds / 0x10000)) + ((dt / 0x10000) * (dt / 0x10000));
      while (nextLength < length2) {
        rampIndex -= 1;
        rampValue = (((int *)ramp))[rampIndex];
        rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
        length2 = (rampIndex - 1) * (rampIndex - 1);
      }
    }
  }

  /* Deal with the last n sub-pixels */
  if (x < x1) {
    colorMask = workBuffer[GWAAColorMask];
    colorShift = workBuffer[GWAAColorShift];
    rampValue = (((int *)ramp))[rampIndex];
    rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
    while (x < x1) {
      while ((x < x1) && ((((ds / 0x10000) * (ds / 0x10000)) +
                           ((dt / 0x10000) * (dt / 0x10000))) >= length2)) {
        index = ((usqInt)(x)) >> baseShift;
        spanBuffer[index] = ((spanBuffer[index]) + rampValue);
        x += 1;
        ds += dsX;
        dt += dtX;
      }

      /* Step to next ramp value */
      nextLength =
          ((ds / 0x10000) * (ds / 0x10000)) + ((dt / 0x10000) * (dt / 0x10000));
      while (nextLength < length2) {
        rampIndex -= 1;
        rampValue = (((int *)ramp))[rampIndex];
        rampValue = ((usqInt)((rampValue & colorMask))) >> colorShift;
        length2 = (rampIndex - 1) * (rampIndex - 1);
      }
    }
  }

  /* Done -- store stuff back */
  (*((int *)deltaST) = ds);
  (((int *)deltaST))[1] = dt;
  return x;
}