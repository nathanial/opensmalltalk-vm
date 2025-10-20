/* Extracted from B2DPlugin.c:8703 (function primitiveRenderAllScanlines). */

EXPORT(sqInt)
primitiveRenderAllScanlines(void) {
  sqInt failCode;
  int stopReason;

  if ((failCode = loadRenderingState())) {
    return primitiveFailFor(failCode);
  }
  stopReason = 0;
  while (!((stopReason != 0) || ((workBuffer[GWState]) == GEStateCompleted))) {
    proceedRenderingScanline();
    stopReason = workBuffer[GWStopReason];
  }
  storeRenderingState();
  return 0;
}