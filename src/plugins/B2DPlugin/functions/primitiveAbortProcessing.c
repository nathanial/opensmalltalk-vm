/* Extracted from B2DPlugin.c:5655 (function primitiveAbortProcessing). */

EXPORT(sqInt)
primitiveAbortProcessing(void) {
  sqInt failureCode;

  if (methodArgumentCount()) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(0)))) {
    return primitiveFailFor(failureCode);
  }

  /* statePut: */
  workBuffer[GWState] = GEStateCompleted;

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  return 0;
}