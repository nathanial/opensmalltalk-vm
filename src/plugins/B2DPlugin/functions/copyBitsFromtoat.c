/* Extracted from B2DPlugin.c:1401 (function copyBitsFromtoat). */

static sqInt copyBitsFromtoat(sqInt x0, sqInt x1, sqInt yValue) {
  if (!copyBitsFn) {
    if (!(initialiseModule())) {
      return 0;
    }
  }

  /* We need copyBits here so try to load it implicitly */
  return ((sqInt (*)(sqInt, sqInt, sqInt))copyBitsFn)(x0, x1, yValue);
}