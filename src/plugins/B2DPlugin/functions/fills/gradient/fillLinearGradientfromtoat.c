/* Extracted from B2DPlugin.c:2246 (function fillLinearGradientfromtoat). */

static sqInt fillLinearGradientfromtoat(sqInt fill, sqInt leftX, sqInt rightX,
                                        sqInt yValue) {
  sqInt ds;
  int dsX;
  int *ramp;
  sqInt rampIndex;
  int rampSize;
  sqInt x;
  sqInt x0;
  sqInt x0SqInt;
  sqInt x1;

  rampIndex = 0;
  ramp = (objBuffer + fill) + GFRampOffset;
  rampSize = objBuffer[fill + GFRampLength];
  dsX = objBuffer[fill + GFDirectionX];
  ds = ((leftX - (objBuffer[fill + GFOriginX])) * dsX) +
       ((yValue - (objBuffer[fill + GFOriginY])) *
        (objBuffer[fill + GFDirectionY]));
  x = (x0 = leftX);
  x1 = rightX;

  /* Note: The inner loop has been divided into three parts for speed
     Part one: Fill everything outside the left boundary */
  while (((((rampIndex = ds / 0x10000)) < 0) || (rampIndex >= rampSize)) &&
         (x < x1)) {
    x += 1;
    ds += dsX;
  }
  if (x > x0) {
    if (rampIndex < 0) {
      rampIndex = 0;
    }
    if (rampIndex >= rampSize) {
      rampIndex = rampSize - 1;
    }

    /* begin fillColorSpan:from:to: */
    if (!((workBuffer[GWAALevel]) == 1)) {
      fillColorSpanAAx0x1(ramp[rampIndex], x0, x);
    } else {
      x0SqInt = x0;

      /* Unroll the inner loop four times, since we're only storing data. */
      while ((x0SqInt + 4) < x) {
        spanBuffer[x0SqInt] = (ramp[rampIndex]);
        spanBuffer[x0SqInt + 1] = (ramp[rampIndex]);
        spanBuffer[x0SqInt + 2] = (ramp[rampIndex]);
        spanBuffer[x0SqInt + 3] = (ramp[rampIndex]);
        x0SqInt += 4;
      }
      while (x0SqInt < x) {
        spanBuffer[x0SqInt] = (ramp[rampIndex]);
        x0SqInt += 1;
      }
    }
    /* end fillColorSpan:from:to: */
  }

  /* Part two: Fill everything inside the boundaries */
  if ((workBuffer[GWAALevel]) == 1) {
    while (((((rampIndex = ds / 0x10000)) < rampSize) && (rampIndex >= 0)) &&
           (x < x1)) {
      spanBuffer[x] = (ramp[rampIndex]);
      x += 1;
      ds += dsX;
    }
  } else {
    x = fillLinearGradientAArampdsdsXfromto(fill, ramp, ds, dsX, x, rightX);
  }

  /* Fast version w/o anti-aliasing
     Part three fill everything outside right boundary */
  if (x < x1) {
    if (rampIndex < 0) {
      rampIndex = 0;
    }
    if (rampIndex >= rampSize) {
      rampIndex = rampSize - 1;
    }

    /* begin fillColorSpan:from:to: */
    if (!((workBuffer[GWAALevel]) == 1)) {
      fillColorSpanAAx0x1(ramp[rampIndex], x, x1);
    } else {
      x0SqInt = x;

      /* Unroll the inner loop four times, since we're only storing data. */
      while ((x0SqInt + 4) < x1) {
        spanBuffer[x0SqInt] = (ramp[rampIndex]);
        spanBuffer[x0SqInt + 1] = (ramp[rampIndex]);
        spanBuffer[x0SqInt + 2] = (ramp[rampIndex]);
        spanBuffer[x0SqInt + 3] = (ramp[rampIndex]);
        x0SqInt += 4;
      }
      while (x0SqInt < x1) {
        spanBuffer[x0SqInt] = (ramp[rampIndex]);
        x0SqInt += 1;
      }
    }
    /* end fillColorSpan:from:to: */
  }
  return 0;
}