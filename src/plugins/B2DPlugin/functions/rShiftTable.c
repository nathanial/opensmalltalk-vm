/* Extracted from B2DPlugin.c:9564 (function rShiftTable). */

static int *rShiftTable(void) {
  static int theTable[17] = {0, 5, 4, 0, 3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1};

  return theTable;
}