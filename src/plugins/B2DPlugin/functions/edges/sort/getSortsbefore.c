/* Extracted from B2DPlugin.c:3448 (function getSortsbefore). */

static sqInt getSortsbefore(sqInt edge1, sqInt edge2) {
  int diff;

  if (edge1 == edge2) {
    return 1;
  }

  /* First, sort by Y */
  diff = (objBuffer[edge1 + GEYValue]) - (objBuffer[edge2 + GEYValue]);
  if (diff) {
    return diff < 0;
  }

  /* Then, by X */
  diff = (objBuffer[edge1 + GEXValue]) - (objBuffer[edge2 + GEXValue]);
  return diff < 0;
}