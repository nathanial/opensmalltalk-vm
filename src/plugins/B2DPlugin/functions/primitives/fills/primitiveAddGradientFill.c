/* Extracted from B2DPlugin.c:6748 (function primitiveAddGradientFill). */

EXPORT(sqInt)
primitiveAddGradientFill(void) {
  sqInt dirOop;
  sqInt failureCode;
  sqInt fill;
  sqInt isRadial;
  sqInt nrmOop;
  sqInt originOop;
  sqInt rampOop;

  if (!((methodArgumentCount()) == 5)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  isRadial = booleanValueOf(stackValue(0));
  nrmOop = stackValue(1);
  dirOop = stackValue(2);
  originOop = stackValue(3);
  rampOop = stackValue(4);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(5), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  loadPointfrom(((int *)(workBuffer + GWPoint1)), originOop);
  loadPointfrom(((int *)(workBuffer + GWPoint2)), dirOop);
  loadPointfrom(((int *)(workBuffer + GWPoint3)), nrmOop);
  if (failed()) {
    return primitiveFailFor(GEFBadPoint);
  }
  fill = loadGradientFillfromalongnormalisRadial(
      rampOop, ((int *)(workBuffer + GWPoint1)),
      ((int *)(workBuffer + GWPoint2)), ((int *)(workBuffer + GWPoint3)),
      isRadial);
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }

  /* Make sure the stack is okay */
  if (failed()) {
    return primitiveFailFor(GEFEntityLoadFailed);
  }

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  popthenPush(6, positive32BitIntegerFor(fill));
  return 0;
}