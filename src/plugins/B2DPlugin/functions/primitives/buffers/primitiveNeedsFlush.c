/* Extracted from B2DPlugin.c:8305 (function primitiveNeedsFlush). */

EXPORT(sqInt)
primitiveNeedsFlush(void) {
  sqInt failureCode;
  int needFlush;

  if (methodArgumentCount()) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(0)))) {
    return primitiveFailFor(failureCode);
  }
  needFlush = (workBuffer[GWNeedsFlush]) != 0;

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  pushBool(needFlush);
  return 0;
}