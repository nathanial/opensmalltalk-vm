/* Extracted from B2DPlugin.c:9577 (function setAALevel). */

static sqInt setAALevel(sqInt level) {
  sqInt aaLevel;

  aaLevel = 0;
  if (level >= 4) {
    aaLevel = 4;
  }
  if ((level >= 2) && (level < 4)) {
    aaLevel = 2;
  }
  if (level < 2) {
    aaLevel = 1;
  }

  /* aaLevelPut: */
  workBuffer[GWAALevel] = aaLevel;
  if (aaLevel == 1) {
    /* aaShiftPut: */
    workBuffer[GWAAShift] = 0;

    /* aaColorMaskPut: */
    workBuffer[GWAAColorMask] = 0xFFFFFFFFU;

    /* aaScanMaskPut: */
    workBuffer[GWAAScanMask] = 0;
  }
  if (aaLevel == 2) {
    /* aaShiftPut: */
    workBuffer[GWAAShift] = 1;

    /* aaColorMaskPut: */
    workBuffer[GWAAColorMask] = 0xFCFCFCFCU;

    /* aaScanMaskPut: */
    workBuffer[GWAAScanMask] = 1;
  }
  if (aaLevel == 4) {
    /* aaShiftPut: */
    workBuffer[GWAAShift] = 2;

    /* aaColorMaskPut: */
    workBuffer[GWAAColorMask] = 0xF0F0F0F0U;

    /* aaScanMaskPut: */
    workBuffer[GWAAScanMask] = 3;
  }

  /* aaColorShiftPut: */
  workBuffer[GWAAColorShift] = ((workBuffer[GWAAShift]) * 2);

  /* aaHalfPixelPut: */
  workBuffer[GWAAHalfPixel] = (workBuffer[GWAAShift]);
  return 0;
}