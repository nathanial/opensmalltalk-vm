/* Extracted from B2DPlugin.c:8919 (function primitiveSetDepth). */

EXPORT(sqInt)
primitiveSetDepth(void) {
  sqInt depth;
  sqInt failureCode;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(1), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  depth = stackIntegerValue(0);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }

  /* currentZPut: */
  workBuffer[GWCurrentZ] = depth;

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  return 0;
}