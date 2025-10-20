/* Extracted from B2DPlugin.c:2845 (function fillSpanfromto). */

static sqInt fillSpanfromto(unsigned int fill, sqInt leftX, sqInt rightX) {
  unsigned int type;
  sqInt x0;
  sqInt x0SqInt;
  sqInt x1;

  if (!fill) {
    return 0;
  }
  if (leftX < (workBuffer[GWSpanEndAA])) {
    x0 = workBuffer[GWSpanEndAA];
  } else {
    x0 = leftX;
  }
  if (rightX >
      ((((usqInt)((workBuffer[GWSpanSize])) << (workBuffer[GWAAShift]))))) {
    x1 = ((sqInt)((usqInt)((workBuffer[GWSpanSize]))
                  << (workBuffer[GWAAShift])));
  } else {
    x1 = rightX;
  }

  /* Clip left and right values */
  if (x0 < (workBuffer[GWFillMinX])) {
    x0 = workBuffer[GWFillMinX];
  }
  if (x1 > (workBuffer[GWFillMaxX])) {
    x1 = workBuffer[GWFillMaxX];
  }

  /* Adjust start and end values of span */
  if (x0 < (workBuffer[GWSpanStart])) {
    /* spanStartPut: */
    workBuffer[GWSpanStart] = x0;
  }
  if (x1 > (workBuffer[GWSpanEnd])) {
    /* spanEndPut: */
    workBuffer[GWSpanEnd] = x1;
  }
  if (x1 > (workBuffer[GWSpanEndAA])) {
    /* spanEndAAPut: */
    workBuffer[GWSpanEndAA] = x1;
  }
  if (x0 >= x1) {
    return 0;
  }
  if (fill & 0xFF000000U) {
    /* begin fillColorSpan:from:to: */
    if (!((workBuffer[GWAALevel]) == 1)) {
      fillColorSpanAAx0x1(fill, x0, x1);
    } else {
      x0SqInt = x0;

      /* Unroll the inner loop four times, since we're only storing data. */
      while ((x0SqInt + 4) < x1) {
        spanBuffer[x0SqInt] = fill;
        spanBuffer[x0SqInt + 1] = fill;
        spanBuffer[x0SqInt + 2] = fill;
        spanBuffer[x0SqInt + 3] = fill;
        x0SqInt += 4;
      }
      while (x0SqInt < x1) {
        spanBuffer[x0SqInt] = fill;
        x0SqInt += 1;
      }
    }
    /* end fillColorSpan:from:to: */
  } else {
    /* lastExportedFillPut: */
    workBuffer[GWLastExportedFill] = fill;

    /* lastExportedLeftXPut: */
    workBuffer[GWLastExportedLeftX] = x0;

    /* lastExportedRightXPut: */
    workBuffer[GWLastExportedRightX] = x1;
    type = ((usqInt)((((objBuffer[fill + GEObjectType]) & GEPrimitiveTypeMask) &
                      GEPrimitiveFillMask))) >>
           8;
    if (type <= 1) {
      return 1;
    }
    switch (type) {
    case 0: // errorWrongIndex
    case 1: // errorWrongIndex
      error("BalloonEngine: Fatal dispatch error");
      break;
    case 2: // fillLinearGradient
      fillLinearGradientfromtoat(
          workBuffer[GWLastExportedFill], workBuffer[GWLastExportedLeftX],
          workBuffer[GWLastExportedRightX], workBuffer[GWCurrentY]);
      break;
    case 3: // fillRadialGradient
      fillRadialGradientfromtoat(
          workBuffer[GWLastExportedFill], workBuffer[GWLastExportedLeftX],
          workBuffer[GWLastExportedRightX], workBuffer[GWCurrentY]);
      break;
    case 4: // fillBitmapSpan
    case 5: // fillBitmapSpan
      fillBitmapSpanfromtoat(
          workBuffer[GWLastExportedFill], workBuffer[GWLastExportedLeftX],
          workBuffer[GWLastExportedRightX], workBuffer[GWCurrentY]);
      break;
    }
  }

  /* Store the values for the dispatch */
  return 0;
}