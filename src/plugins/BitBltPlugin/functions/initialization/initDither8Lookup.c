/* Extracted from BitBltPlugin.c:2942 (function initDither8Lookup). */
/* BitBltSimulation>>#initDither8Lookup */

static sqInt initDither8Lookup(void) {
  sqInt b;
  int out;
  unsigned int pv;
  sqInt t;
  int threshold;
  unsigned int value;
  int valueInt;

  for (b = 0; b <= 0xFF; b += 1) {
    for (t = 0; t <= 15; t += 1) {
      /* begin expensiveDither32To16:threshold: */
      pv = (((unsigned int)b)) & 0xFF;
      threshold = ditherThresholds16[pv & 7];
      valueInt = ditherValues16[(((usqInt)(pv)) >> 3)];
      if (t < threshold) {
        out = valueInt + 1;
      } else {
        out = valueInt;
      }
      pv = ((((usqInt)((((unsigned int)b)))) >> 8)) & 0xFF;
      threshold = ditherThresholds16[pv & 7];
      valueInt = ditherValues16[(((usqInt)(pv)) >> 3)];
      if (t < threshold) {
        out = out | (((((usqInt)((valueInt + 1)) << 5))));
      } else {
        out = out | (((((usqInt)(valueInt) << 5))));
      }
      pv = ((((usqInt)((((unsigned int)b)))) >> 16)) & 0xFF;
      threshold = ditherThresholds16[pv & 7];
      valueInt = ditherValues16[(((usqInt)(pv)) >> 3)];
      if (t < threshold) {
        out = out | (((((usqInt)((valueInt + 1)) << 10))));
      } else {
        out = out | (((((usqInt)(valueInt) << 10))));
      }
      value = out;
      dither8Lookup[((((usqInt)(t) << 8))) + b] = value;
    }
  }
  return 0;
}
