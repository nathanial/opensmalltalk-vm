/* Extracted from B2DPlugin.c:8742 (function primitiveRenderScanline). */

EXPORT(sqInt)
primitiveRenderScanline(void) {
  sqInt failCode;

  if ((failCode = loadRenderingState())) {
    return primitiveFailFor(failCode);
  }
  proceedRenderingScanline();
  storeRenderingState();
  return 0;
}