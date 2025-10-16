/* Extracted from B2DPlugin.c:3998 (function loadBitBltFrom). */

static sqInt loadBitBltFrom(sqInt bbObj) {
  if (!loadBBFn) {
    if (!(initialiseModule())) {
      return 0;
    }
  }

  /* We need copyBits here so try to load it implicitly */
  return ((sqInt (*)(sqInt))loadBBFn)(bbObj);
}