/* Extracted from B2DPlugin.c:8497 (function primitiveNextGlobalEdgeEntry). */

EXPORT(sqInt)
primitiveNextGlobalEdgeEntry(void) {
  int edge;
  sqInt edgeOop;
  sqInt failureCode;
  sqInt hasEdge;
  sqInt value;

  if (doProfileStats) {
    geProfileTime = ioMicroMSecs();
  }
  if (!((methodArgumentCount()) == 1)) {
    return primitiveFailFor(PrimErrBadNumArgs);
  }
  if ((failureCode = quickLoadEngineFromrequiredState(stackValue(1),
                                                      GEStateAddingFromGET))) {
    return primitiveFailFor(failureCode);
  }
  edgeOop = stackObjectValue(0);
  hasEdge = findNextExternalEntryFromGET();
  if (hasEdge) {
    edge = getBuffer[workBuffer[GWGETStart]];
    storeEdgeStateFrominto(edge, edgeOop);

    /* getStartPut: */
    workBuffer[GWGETStart] = ((workBuffer[GWGETStart]) + 1);
  }
  if (failed()) {
    return primitiveFailFor(GEFWrongEdge);
  }
  if (hasEdge) {
    /* statePut: */
    workBuffer[GWState] = GEStateWaitingForEdge;
  } else {
    /* statePut: */
    workBuffer[GWState] = GEStateScanningAET;

    /* clearSpanBufferPut: */
    workBuffer[GWClearSpanBuffer] = 1;

    /* aetStartPut: */
    workBuffer[GWAETStart] = 0;

    /* begin wbStackClear */
    /* wbTopPut: */
    workBuffer[GWBufferTop] = (workBuffer[GWSize]);
  }

  /* Start scanning the AET */
  workBuffer[GWObjUsed] = objUsed;
  pop(2);
  pushBool(!hasEdge);
  if (doProfileStats) {
    /* incrementStat:by: */
    workBuffer[GWCountNextGETEntry] = ((workBuffer[GWCountNextGETEntry]) + 1);
    value = (ioMicroMSecs()) - geProfileTime;

    /* begin incrementStat:by: */
    workBuffer[GWTimeNextGETEntry] = ((workBuffer[GWTimeNextGETEntry]) + value);
  }
  return 0;
}