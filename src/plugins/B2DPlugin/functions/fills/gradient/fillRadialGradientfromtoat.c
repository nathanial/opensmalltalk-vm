/* Extracted from B2DPlugin.c:2493 (function fillRadialGradientfromtoat). */

static sqInt fillRadialGradientfromtoat(sqInt fill, sqInt leftX, sqInt rightX,
                                        sqInt yValue) {
  int *deltaST;
  sqInt deltaX;
  sqInt deltaY;
  sqInt ds;
  int dsInt;
  int dsX;
  sqInt dt;
  int dtInt;
  int dtX;
  sqInt lastLength;
  int length2;
  int length2Int;
  sqInt length2SqInt;
  sqInt nextLength;
  int *ramp;
  sqInt rampIndex;
  int rampSize;
  int rampSizeInt;
  sqInt rampValue;
  sqInt x;
  sqInt x0;
  sqInt x1;
  sqInt x1SqInt;
  sqInt xSqInt;

  ramp = (objBuffer + fill) + GFRampOffset;
  rampSize = objBuffer[fill + GFRampLength];
  deltaX = leftX - (objBuffer[fill + GFOriginX]);
  deltaY = yValue - (objBuffer[fill + GFOriginY]);
  dsX = objBuffer[fill + GFDirectionX];
  dtX = objBuffer[fill + GFNormalX];
  ds = (deltaX * dsX) + (deltaY * (objBuffer[fill + GFDirectionY]));
  dt = (deltaX * dtX) + (deltaY * (objBuffer[fill + GFNormalY]));
  x = leftX;
  x1 = rightX;

  /* Note: The inner loop has been divided into three parts for speed
     Part one: Fill everything outside the left boundary */

  /* This is the upper bound */
  length2 = (rampSize - 1) * (rampSize - 1);
  while (((((ds / 0x10000) * (ds / 0x10000)) +
           ((dt / 0x10000) * (dt / 0x10000))) >= length2) &&
         (x < x1)) {
    x += 1;
    ds += dsX;
    dt += dtX;
  }
  if (x > leftX) {
    /* begin fillColorSpan:from:to: */
    if (!((workBuffer[GWAALevel]) == 1)) {
      fillColorSpanAAx0x1(ramp[rampSize - 1], leftX, x);
    } else {
      x0 = leftX;

      /* Unroll the inner loop four times, since we're only storing data. */
      while ((x0 + 4) < x) {
        spanBuffer[x0] = (ramp[rampSize - 1]);
        spanBuffer[x0 + 1] = (ramp[rampSize - 1]);
        spanBuffer[x0 + 2] = (ramp[rampSize - 1]);
        spanBuffer[x0 + 3] = (ramp[rampSize - 1]);
        x0 += 4;
      }
      while (x0 < x) {
        spanBuffer[x0] = (ramp[rampSize - 1]);
        x0 += 1;
      }
    }
    /* end fillColorSpan:from:to: */
  }

  /* Part two: Fill everything inside the boundaries */
  deltaST = ((int *)(workBuffer + GWPoint1));
  deltaST[0] = ds;
  deltaST[1] = dt;
  if (x < (objBuffer[fill + GFOriginX])) {
    if ((workBuffer[GWAALevel]) == 1) {
      /* begin fillRadialDecreasing:ramp:deltaST:dsX:dtX:from:to: */
      dsInt = (*((int *)deltaST));
      dtInt = (((int *)deltaST))[1];
      rampIndex = accurateLengthOfwith(dsInt / 0x10000, dtInt / 0x10000);
      rampValue = (((int *)ramp))[rampIndex];
      length2SqInt = (rampIndex - 1) * (rampIndex - 1);
      xSqInt = x;
      x1SqInt = x1;
      if (x1SqInt > (objBuffer[fill + GFOriginX])) {
        x1SqInt = objBuffer[fill + GFOriginX];
      }
      while (xSqInt < x1SqInt) {
        while ((xSqInt < x1SqInt) &&
               ((((dsInt / 0x10000) * (dsInt / 0x10000)) +
                 ((dtInt / 0x10000) * (dtInt / 0x10000))) >= length2SqInt)) {
          spanBuffer[xSqInt] = rampValue;
          xSqInt += 1;
          dsInt += dsX;
          dtInt += dtX;
        }

        /* Step to next ramp value */
        nextLength = ((dsInt / 0x10000) * (dsInt / 0x10000)) +
                     ((dtInt / 0x10000) * (dtInt / 0x10000));
        while (nextLength < length2SqInt) {
          rampIndex -= 1;
          rampValue = (((int *)ramp))[rampIndex];
          length2SqInt = (rampIndex - 1) * (rampIndex - 1);
        }
      }

      /* Try to copy the current value more than just once */
      (*((int *)deltaST) = dsInt);
      (((int *)deltaST))[1] = dtInt;
      x = xSqInt;
    } else {
      x = fillRadialDecreasingAArampdeltaSTdsXdtXfromto(fill, ramp, deltaST,
                                                        dsX, dtX, x, x1);
    }
  }

  /* Draw the decreasing part */
  if (x < x1) {
    if ((workBuffer[GWAALevel]) == 1) {
      /* begin fillRadialIncreasing:ramp:deltaST:dsX:dtX:from:to: */
      dsInt = (*((int *)deltaST));
      dtInt = (((int *)deltaST))[1];
      rampIndex = accurateLengthOfwith(dsInt / 0x10000, dtInt / 0x10000);
      rampValue = (((int *)ramp))[rampIndex];
      rampSizeInt = objBuffer[fill + GFRampLength];

      /* This is the upper bound */
      length2Int = (rampSizeInt - 1) * (rampSizeInt - 1);
      nextLength = (rampIndex + 1) * (rampIndex + 1);
      lastLength = ((dsInt / 0x10000) * (dsInt / 0x10000)) +
                   ((dtInt / 0x10000) * (dtInt / 0x10000));
      xSqInt = x;
      x1SqInt = x1;
      while ((xSqInt < x1SqInt) && (lastLength < length2Int)) {
        while ((xSqInt < x1SqInt) &&
               ((((dsInt / 0x10000) * (dsInt / 0x10000)) +
                 ((dtInt / 0x10000) * (dtInt / 0x10000))) <= nextLength)) {
          spanBuffer[xSqInt] = rampValue;
          xSqInt += 1;
          dsInt += dsX;
          dtInt += dtX;
        }
        lastLength = ((dsInt / 0x10000) * (dsInt / 0x10000)) +
                     ((dtInt / 0x10000) * (dtInt / 0x10000));
        while (lastLength > nextLength) {
          rampIndex += 1;
          rampValue = (((int *)ramp))[rampIndex];
          nextLength = (rampIndex + 1) * (rampIndex + 1);
        }
      }

      /* Try to copy the current value more than once */
      (*((int *)deltaST) = dsInt);
      (((int *)deltaST))[1] = dtInt;
      x = xSqInt;
    } else {
      x = fillRadialIncreasingAArampdeltaSTdsXdtXfromto(fill, ramp, deltaST,
                                                        dsX, dtX, x, x1);
    }
  }

  /* Draw the increasing part
     Part three fill everything outside right boundary */
  if (x < rightX) {
    /* begin fillColorSpan:from:to: */
    if (!((workBuffer[GWAALevel]) == 1)) {
      fillColorSpanAAx0x1(ramp[rampSize - 1], x, rightX);
    } else {
      x0 = x;

      /* Unroll the inner loop four times, since we're only storing data. */
      while ((x0 + 4) < rightX) {
        spanBuffer[x0] = (ramp[rampSize - 1]);
        spanBuffer[x0 + 1] = (ramp[rampSize - 1]);
        spanBuffer[x0 + 2] = (ramp[rampSize - 1]);
        spanBuffer[x0 + 3] = (ramp[rampSize - 1]);
        x0 += 4;
      }
      while (x0 < rightX) {
        spanBuffer[x0] = (ramp[rampSize - 1]);
        x0 += 1;
      }
    }
    /* end fillColorSpan:from:to: */
  }
  return 0;
}