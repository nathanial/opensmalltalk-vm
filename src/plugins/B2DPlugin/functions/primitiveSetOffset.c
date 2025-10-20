/* Extracted from B2DPlugin.c:8976 (function primitiveSetOffset). */

EXPORT(sqInt)
primitiveSetOffset(void) {
  sqInt failureCode;
  sqInt pointOop;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(1), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  pointOop = stackValue(0);
  if (!((fetchClassOf(pointOop)) == (classPoint()))) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  loadPointfrom(((int *)(workBuffer + GWPoint1)), pointOop);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }

  /* destOffsetXPut: */
  workBuffer[GWDestOffsetX] = ((*((int *)(workBuffer + GWPoint1))));

  /* destOffsetYPut: */
  workBuffer[GWDestOffsetY] = ((((int *)(workBuffer + GWPoint1)))[1]);

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  return 0;
}