/* Extracted from B2DPlugin.c:8177 (function primitiveInitializeProcessing). */

EXPORT(sqInt)
primitiveInitializeProcessing(void) {
  sqInt failureCode;
  sqInt value;

  if (doProfileStats) {
    geProfileTime = ioMicroMSecs();
  }
  if (methodArgumentCount()) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode =
           quickLoadEngineFromrequiredState(stackValue(0), GEStateUnlocked))) {
    return primitiveFailFor(failureCode);
  }

  /* Load span buffer for clear operation */
  if ((failureCode =
           loadSpanBufferFrom(fetchPointerofObject(BESpanIndex, engine)))) {
    return primitiveFailFor(failureCode);
  }
  initializeGETProcessing();
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }

  /* statePut: */
  workBuffer[GWState] = GEStateAddingFromGET;
  if (!(failed())) {
    /* begin storeEngineStateInto: */
    /* objUsedPut: */
    workBuffer[GWObjUsed] = objUsed;
  }
  if (doProfileStats) {
    /* incrementStat:by: */
    workBuffer[GWCountInitializing] = ((workBuffer[GWCountInitializing]) + 1);
    value = (ioMicroMSecs()) - geProfileTime;

    /* begin incrementStat:by: */
    workBuffer[GWTimeInitializing] = ((workBuffer[GWTimeInitializing]) + value);
  }
  return 0;
}