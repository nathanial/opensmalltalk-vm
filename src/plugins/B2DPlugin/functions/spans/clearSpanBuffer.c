/* Extracted from B2DPlugin.c:1042 (function clearSpanBuffer). */

static sqInt clearSpanBuffer(void) {
  int x0;
  int x1;

  x0 = ((usqInt)((workBuffer[GWSpanStart]))) >> (workBuffer[GWAAShift]);
  x1 = (((usqInt)((workBuffer[GWSpanEnd]))) >> (workBuffer[GWAAShift])) + 1;
  if (x0 < 0) {
    x0 = 0;
  }
  if (x1 > (workBuffer[GWSpanSize])) {
    x1 = workBuffer[GWSpanSize];
  }
  while (x0 < x1) {
    spanBuffer[x0] = 0;
    x0 += 1;
  }

  /* spanStartPut: */
  workBuffer[GWSpanStart] = (workBuffer[GWSpanSize]);

  /* spanEndPut: */
  workBuffer[GWSpanEnd] = 0;
  return 0;
}