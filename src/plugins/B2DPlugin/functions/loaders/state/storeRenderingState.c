/* Extracted from B2DPlugin.c:10423 (function storeRenderingState). */

static sqInt storeRenderingState(void) {
  int edge;
  sqInt edgeOop;
  sqInt fillOop;
  int reason;

  if (failed()) {
    return null;
  }
  if (engineStopped) {
    edgeOop = stackObjectValue(1);
    fillOop = stackObjectValue(0);

    /* begin storeStopStateIntoEdge:fill: */
    reason = workBuffer[GWStopReason];
    if (reason == GErrorGETEntry) {
      edge = getBuffer[workBuffer[GWGETStart]];
      storeEdgeStateFrominto(edge, edgeOop);

      /* getStartPut: */
      workBuffer[GWGETStart] = ((workBuffer[GWGETStart]) + 1);
    }
    if (reason == GErrorFillEntry) {
      storeFillStateInto(fillOop);
    }
    if (reason == GErrorAETEntry) {
      edge = aetBuffer[workBuffer[GWAETStart]];
      storeEdgeStateFrominto(edge, edgeOop);
    }
  }

  /* Check the stop reason and store the required information */

  /* begin storeEngineStateInto: */
  /* objUsedPut: */
  workBuffer[GWObjUsed] = objUsed;
  methodReturnInteger(workBuffer[GWStopReason]);
  return 0;
}