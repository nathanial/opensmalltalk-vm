/* Extracted from B2DPlugin.c:8755 (function primitiveSetAALevel). */

EXPORT(sqInt)
primitiveSetAALevel(void) {
  sqInt failureCode;
  sqInt level;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(1), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }
  level = stackIntegerValue(0);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  setAALevel(level);

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  return 0;
}