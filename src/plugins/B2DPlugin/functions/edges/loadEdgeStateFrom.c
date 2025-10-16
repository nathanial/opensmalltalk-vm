/* Extracted from B2DPlugin.c:4596 (function loadEdgeStateFrom). */

static int loadEdgeStateFrom(sqInt edgeOop) {
  int edge;
  sqInt value;

  edge = workBuffer[GWLastExportedEdge];
  if ((slotSizeOf(edgeOop)) < ETBalloonEdgeDataSize) {
    return null;
  }
  value = fetchIntegerofObject(ETXValueIndex, edgeOop);

  /* begin edgeXValueOf:put: */
  /* obj:at:put: */
  objBuffer[edge + GEXValue] = value;
  value = fetchIntegerofObject(ETYValueIndex, edgeOop);

  /* begin edgeYValueOf:put: */
  /* obj:at:put: */
  objBuffer[edge + GEYValue] = value;
  value = fetchIntegerofObject(ETZValueIndex, edgeOop);

  /* begin edgeZValueOf:put: */
  /* obj:at:put: */
  objBuffer[edge + GEZValue] = value;
  value = fetchIntegerofObject(ETLinesIndex, edgeOop);

  /* begin edgeNumLinesOf:put: */
  /* obj:at:put: */
  objBuffer[edge + GENumLines] = value;
  return edge;
}