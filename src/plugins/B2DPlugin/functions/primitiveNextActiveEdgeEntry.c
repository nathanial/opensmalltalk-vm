/* Extracted from B2DPlugin.c:8356 (function primitiveNextActiveEdgeEntry). */

EXPORT(sqInt)
primitiveNextActiveEdgeEntry(void) {
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
  if ((failureCode = quickLoadEngineFromrequiredStateor(
           stackValue(1), GEStateUpdateEdges, GEStateCompleted))) {
    return primitiveFailFor(failureCode);
  }
  edgeOop = stackObjectValue(0);
  if (failed()) {
    return primitiveFailFor(PrimErrBadArgument);
  }
  hasEdge = 0;
  if (!((workBuffer[GWState]) == GEStateCompleted)) {
    hasEdge = findNextExternalUpdateFromAET();
    if (hasEdge) {
      edge = aetBuffer[workBuffer[GWAETStart]];
      storeEdgeStateFrominto(edge, edgeOop);

      /* Do not advance to the next aet entry yet
         self aetStartPut: self aetStartGet + 1. */

      /* statePut: */
      workBuffer[GWState] = GEStateWaitingChange;
    } else {
      /* statePut: */
      workBuffer[GWState] = GEStateAddingFromGET;
    }
  }
  if (failed()) {
    return null;
  }

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  pop(2);
  pushBool(!hasEdge);
  if (doProfileStats) {
    /* incrementStat:by: */
    workBuffer[GWCountNextAETEntry] = ((workBuffer[GWCountNextAETEntry]) + 1);
    value = (ioMicroMSecs()) - geProfileTime;

    /* begin incrementStat:by: */
    workBuffer[GWTimeNextAETEntry] = ((workBuffer[GWTimeNextAETEntry]) + value);
  }
  return 0;
}