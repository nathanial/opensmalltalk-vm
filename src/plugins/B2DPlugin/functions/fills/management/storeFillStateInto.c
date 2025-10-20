/* Extracted from B2DPlugin.c:10403 (function storeFillStateInto). */

static sqInt storeFillStateInto(sqInt fillOop) {
  int fillIndex;
  int leftX;
  int rightX;

  fillIndex = workBuffer[GWLastExportedFill];
  leftX = workBuffer[GWLastExportedLeftX];
  rightX = workBuffer[GWLastExportedRightX];
  if ((slotSizeOf(fillOop)) < FTBalloonFillDataSize) {
    return primitiveFail();
  }
  storeIntegerofObjectwithValue(FTIndexIndex, fillOop,
                                objBuffer[fillIndex + GEObjectIndex]);
  storeIntegerofObjectwithValue(FTMinXIndex, fillOop, leftX);
  storeIntegerofObjectwithValue(FTMaxXIndex, fillOop, rightX);
  storeIntegerofObjectwithValue(FTYValueIndex, fillOop, workBuffer[GWCurrentY]);
  return 0;
}