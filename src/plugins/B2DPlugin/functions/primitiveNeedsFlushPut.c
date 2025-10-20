/* Extracted from B2DPlugin.c:8327 (function primitiveNeedsFlushPut). */

EXPORT(sqInt)
primitiveNeedsFlushPut(void) {
  sqInt failureCode;
  sqInt needFlush;

  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(1)))) {
    return primitiveFailFor(failureCode);
  }
  needFlush = booleanValueOf(stackValue(0));
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }

  /* needsFlushPut: */
  workBuffer[GWNeedsFlush] = ((needFlush ? 1 : 0));

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  return 0;
}