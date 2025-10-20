/* Extracted from B2DPlugin.c:9545 (function resortFirstAETEntry). */

static sqInt resortFirstAETEntry(void) {
  int edge;
  int leftEdge;
  int xValue;

  if (!(workBuffer[GWAETStart])) {
    return null;
  }
  edge = aetBuffer[workBuffer[GWAETStart]];
  xValue = objBuffer[edge + GEXValue];
  leftEdge = aetBuffer[(workBuffer[GWAETStart]) - 1];
  if ((objBuffer[leftEdge + GEXValue]) <= xValue) {
    return null;
  }
  moveAETEntryFromedgex(workBuffer[GWAETStart], edge, xValue);
  return 0;
}