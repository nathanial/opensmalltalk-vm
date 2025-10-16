/* Extracted from B2DPlugin.c:8723 (function primitiveRenderImage). */

EXPORT(sqInt)
primitiveRenderImage(void) {
  sqInt failCode;

  if ((failCode = loadRenderingState())) {
    return primitiveFailFor(failCode);
  }
  proceedRenderingScanline();
  if (engineStopped) {
    return storeRenderingState();
  }
  proceedRenderingImage();
  storeRenderingState();
  return 0;
}