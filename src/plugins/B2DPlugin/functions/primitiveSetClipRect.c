/* Extracted from B2DPlugin.c:8825 (function primitiveSetClipRect). */

EXPORT(sqInt)
primitiveSetClipRect(void) {
  sqInt failureCode;
  sqInt rectOop;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(1), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  rectOop = stackObjectValue(0);
  if (!((!(failed())) &&
        ((isPointers(rectOop)) && ((slotSizeOf(rectOop)) >= 2)))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  loadPointfrom(((int *)(workBuffer + GWPoint1)),
                fetchPointerofObject(0, rectOop));
  loadPointfrom(((int *)(workBuffer + GWPoint2)),
                fetchPointerofObject(1, rectOop));
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }

  /* clipMinXPut: */
  workBuffer[GWClipMinX] = ((*((int *)(workBuffer + GWPoint1))));

  /* clipMinYPut: */
  workBuffer[GWClipMinY] = ((((int *)(workBuffer + GWPoint1)))[1]);

  /* clipMaxXPut: */
  workBuffer[GWClipMaxX] = ((*((int *)(workBuffer + GWPoint2))));

  /* clipMaxYPut: */
  workBuffer[GWClipMaxY] = ((((int *)(workBuffer + GWPoint2)))[1]);

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  return 0;
}