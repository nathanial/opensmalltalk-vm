/* Extracted from B2DPlugin.c:7601 (function primitiveChangedActiveEdgeEntry). */

EXPORT(sqInt)
primitiveChangedActiveEdgeEntry(void) {
  int edge;
  sqInt edgeOop;
  sqInt failureCode;
  sqInt value;

  if (doProfileStats) {
    geProfileTime = ioMicroMSecs();
  }
  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFromrequiredState(stackValue(1),
                                                      GEStateWaitingChange))) {
    return primitiveFailFor(failureCode);
  }
  edgeOop = stackObjectValue(0);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  edge = loadEdgeStateFrom(edgeOop);
  if (!edge) {
    return primitiveFailFor(GEFEdgeDataTooSmall);
  }
  if (objBuffer[edge + GENumLines]) {
    resortFirstAETEntry();

    /* aetStartPut: */
    workBuffer[GWAETStart] = ((workBuffer[GWAETStart]) + 1);
  } else {
    removeFirstAETEntry();
  }

  /* statePut: */
  workBuffer[GWState] = GEStateUpdateEdges;

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  if (doProfileStats) {
    /* incrementStat:by: */
    workBuffer[GWCountChangeAETEntry] =
        ((workBuffer[GWCountChangeAETEntry]) + 1);
    value = (ioMicroMSecs()) - geProfileTime;

    /* begin incrementStat:by: */
    workBuffer[GWTimeChangeAETEntry] =
        ((workBuffer[GWTimeChangeAETEntry]) + value);
  }
  return 0;
}