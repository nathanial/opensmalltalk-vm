/* Extracted from B2DPlugin.c:7932 (function primitiveGetClipRect). */

EXPORT(sqInt)
primitiveGetClipRect(void) {
  sqInt failureCode;
  sqInt pointOop;
  sqInt rectOop;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(1)))) {
    return primitiveFailFor(failureCode);
  }
  rectOop = stackObjectValue(0);
  if (!((!(failed())) &&
        ((isPointers(rectOop)) && ((slotSizeOf(rectOop)) >= 2)))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  pushRemappableOop(rectOop);
  pointOop =
      makePointwithxValueyValue(workBuffer[GWClipMinX], workBuffer[GWClipMinY]);
  storePointerofObjectwithValue(0, topRemappableOop(), pointOop);
  pointOop =
      makePointwithxValueyValue(workBuffer[GWClipMaxX], workBuffer[GWClipMaxY]);
  rectOop = popRemappableOop();
  storePointerofObjectwithValue(1, rectOop, pointOop);
  popthenPush(2, rectOop);
  return 0;
}