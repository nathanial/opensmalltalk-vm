/* Extracted from B2DPlugin.c:7796 (function primitiveFinishedProcessing). */

EXPORT(sqInt)
primitiveFinishedProcessing(void) {
  sqInt failureCode;
  int finished;
  sqInt value;

  if (doProfileStats) {
    geProfileTime = ioMicroMSecs();
  }
  if (methodArgumentCount()) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFrom(stackValue(0)))) {
    return primitiveFailFor(failureCode);
  }
  finished = (workBuffer[GWState]) == GEStateCompleted;

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  pushBool(finished);
  if (doProfileStats) {
    /* incrementStat:by: */
    workBuffer[GWCountFinishTest] = ((workBuffer[GWCountFinishTest]) + 1);
    value = (ioMicroMSecs()) - geProfileTime;

    /* begin incrementStat:by: */
    workBuffer[GWTimeFinishTest] = ((workBuffer[GWTimeFinishTest]) + value);
  }
  return 0;
}