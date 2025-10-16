/* Extracted from B2DPlugin.c:10383 (function storeEdgeStateFrominto). */

static sqInt storeEdgeStateFrominto(sqInt edge, sqInt edgeOop) {
  if ((slotSizeOf(edgeOop)) < ETBalloonEdgeDataSize) {
    return primitiveFail();
  }
  storeIntegerofObjectwithValue(ETIndexIndex, edgeOop,
                                objBuffer[edge + GEObjectIndex]);
  storeIntegerofObjectwithValue(ETXValueIndex, edgeOop,
                                objBuffer[edge + GEXValue]);
  storeIntegerofObjectwithValue(ETYValueIndex, edgeOop, workBuffer[GWCurrentY]);
  storeIntegerofObjectwithValue(ETZValueIndex, edgeOop,
                                objBuffer[edge + GEZValue]);
  storeIntegerofObjectwithValue(ETLinesIndex, edgeOop,
                                objBuffer[edge + GENumLines]);

  /* lastExportedEdgePut: */
  workBuffer[GWLastExportedEdge] = edge;
  return 0;
}