/* Extracted from B2DPlugin.c:855 (function checkCompressedFills). */

static sqInt checkCompressedFills(sqInt indexList) {
  int fillIndex;
  int *fillPtr;
  sqInt i;
  sqInt length;

  /* First check if the oops have the right format */
  if (!(isWords(indexList))) {
    return 0;
  }

  /* Then check the fill entries */
  length = slotSizeOf(indexList);
  fillPtr = firstIndexableField(indexList);
  for (i = 0; i < length; i += 1) {
    fillIndex = fillPtr[i];

    /* Make sure the fill is okay */
    if (!(isFillOkay(fillIndex))) {
      return 0;
    }
  }
  return 1;
}