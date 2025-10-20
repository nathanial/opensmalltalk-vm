/* Extracted from B2DPlugin.c:8947 (function primitiveSetEdgeTransform). */

EXPORT(sqInt)
primitiveSetEdgeTransform(void) {
  sqInt failureCode;
  sqInt transformOop;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(1), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  transformOop = stackObjectValue(0);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  loadEdgeTransformFrom(transformOop);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  return 0;
}