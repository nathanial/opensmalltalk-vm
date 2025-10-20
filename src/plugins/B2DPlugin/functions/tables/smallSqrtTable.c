/* Extracted from B2DPlugin.c:9791 (function smallSqrtTable). */

static int *smallSqrtTable(void) {
  static int theTable[32] = {0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4,
                             4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6};

  return theTable;
}