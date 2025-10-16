/* Extracted from B2DPlugin.c:1885 (function fillBitmapSpanfromto). */

static sqInt fillBitmapSpanfromto(int *bits, sqInt leftX, sqInt rightX) {
  int baseShift;
  sqInt bitX;
  int colorMask;
  int colorShift;
  sqInt fillValue;
  sqInt x;
  sqInt x0;
  sqInt x1;

  x0 = leftX;
  x1 = rightX;

  /* Hack for pre-increment */
  bitX = -1;
  if ((workBuffer[GWAALevel]) == 1) {
    while (x0 < x1) {
      fillValue = (((int *)bits))[(bitX += 1)];
      spanBuffer[x0] = fillValue;
      x0 += 1;
    }
  } else {
    colorMask = workBuffer[GWAAColorMask];
    colorShift = workBuffer[GWAAColorShift];
    baseShift = workBuffer[GWAAShift];
    while (x0 < x1) {
      x = ((usqInt)(x0)) >> baseShift;
      fillValue = (((int *)bits))[(bitX += 1)];
      fillValue = ((usqInt)((fillValue & colorMask))) >> colorShift;
      spanBuffer[x] = ((spanBuffer[x]) + fillValue);
      x0 += 1;
    }
  }

  /* Speedy version for no anti-aliasing
     Generic version with anti-aliasing */
  if (x1 > (workBuffer[GWSpanEnd])) {
    /* spanEndPut: */
    workBuffer[GWSpanEnd] = x1;
  }
  if (x1 > (workBuffer[GWSpanEndAA])) {
    /* spanEndAAPut: */
    workBuffer[GWSpanEndAA] = x1;
  }
  return 0;
}