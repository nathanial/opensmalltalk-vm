/* Extracted from B2DPlugin.c:5678 (function primitiveAddActiveEdgeEntry). */

EXPORT(sqInt)
primitiveAddActiveEdgeEntry(void) {
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
                                                      GEStateWaitingForEdge))) {
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

  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       1) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return primitiveFailFor(GEFWorkTooBig);
  }
  if ((objBuffer[edge + GENumLines]) > 0) {
    insertEdgeIntoAET(edge);
  }
  if (engineStopped) {
    return primitiveFailFor(GEFEngineStopped);
  }

  /* statePut: */
  workBuffer[GWState] = GEStateAddingFromGET;

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(1);
  if (doProfileStats) {
    /* incrementStat:by: */
    workBuffer[GWCountAddAETEntry] = ((workBuffer[GWCountAddAETEntry]) + 1);
    value = (ioMicroMSecs()) - geProfileTime;

    /* begin incrementStat:by: */
    workBuffer[GWTimeAddAETEntry] = ((workBuffer[GWTimeAddAETEntry]) + value);
  }
  return 0;
}