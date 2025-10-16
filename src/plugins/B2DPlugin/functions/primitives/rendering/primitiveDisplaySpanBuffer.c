/* Extracted from B2DPlugin.c:7722 (function primitiveDisplaySpanBuffer). */

EXPORT(sqInt)
primitiveDisplaySpanBuffer(void) {
  sqInt failureCode;
  sqInt value;

  if (doProfileStats) {
    geProfileTime = ioMicroMSecs();
  }
  if (methodArgumentCount()) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFromrequiredState(stackValue(0),
                                                      GEStateBlitBuffer))) {
    return primitiveFailFor(failureCode);
  }

  /* Load span buffer and bitBlt */
  if ((failureCode =
           loadSpanBufferFrom(fetchPointerofObject(BESpanIndex, engine)))) {
    return primitiveFailFor(failureCode);
  }
  if (!(loadBitBltFrom(fetchPointerofObject(BEBitBltIndex, engine)))) {
    return primitiveFailFor(GEFBitBltLoadFailed);
  }
  if (((workBuffer[GWCurrentY]) & (workBuffer[GWAAScanMask])) ==
      (workBuffer[GWAAScanMask])) {
    displaySpanBufferAt(workBuffer[GWCurrentY]);
    postDisplayAction();
  }
  if (!((workBuffer[GWState]) == GEStateCompleted)) {
    /* aetStartPut: */
    workBuffer[GWAETStart] = 0;

    /* currentYPut: */
    workBuffer[GWCurrentY] = ((workBuffer[GWCurrentY]) + 1);

    /* statePut: */
    workBuffer[GWState] = GEStateUpdateEdges;
  }

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  if (doProfileStats) {
    /* incrementStat:by: */
    workBuffer[GWCountDisplaySpan] = ((workBuffer[GWCountDisplaySpan]) + 1);
    value = (ioMicroMSecs()) - geProfileTime;

    /* begin incrementStat:by: */
    workBuffer[GWTimeDisplaySpan] = ((workBuffer[GWTimeDisplaySpan]) + value);
  }
  return 0;
}