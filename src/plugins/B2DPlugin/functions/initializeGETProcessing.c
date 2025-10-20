/* Extracted from B2DPlugin.c:3644 (function initializeGETProcessing). */

static sqInt initializeGETProcessing(void) {
  sqInt value;

  setAALevel(workBuffer[GWAALevel]);
  if ((workBuffer[GWClipMinX]) < 0) {
    /* clipMinXPut: */
    workBuffer[GWClipMinX] = 0;
  }
  if ((workBuffer[GWClipMaxX]) > (workBuffer[GWSpanSize])) {
    /* clipMaxXPut: */
    workBuffer[GWClipMaxX] = (workBuffer[GWSpanSize]);
  }

  /* Convert clipRect to aaLevel */

  /* fillMinXPut: */
  workBuffer[GWFillMinX] = ((
      (sqInt)((usqInt)((workBuffer[GWClipMinX])) << (workBuffer[GWAAShift]))));

  /* fillMinYPut: */
  workBuffer[GWFillMinY] = ((
      (sqInt)((usqInt)((workBuffer[GWClipMinY])) << (workBuffer[GWAAShift]))));

  /* fillMaxXPut: */
  workBuffer[GWFillMaxX] = ((
      (sqInt)((usqInt)((workBuffer[GWClipMaxX])) << (workBuffer[GWAAShift]))));

  /* fillMaxYPut: */
  workBuffer[GWFillMaxY] = ((
      (sqInt)((usqInt)((workBuffer[GWClipMaxY])) << (workBuffer[GWAAShift]))));

  /* Reset GET and AET */

  /* getUsedPut: */
  workBuffer[GWGETUsed] = 0;

  /* aetUsedPut: */
  workBuffer[GWAETUsed] = 0;
  getBuffer = objBuffer + objUsed;
  aetBuffer = objBuffer + objUsed;

  /* Create the global edge table */
  createGlobalEdgeTable();
  if (engineStopped) {
    return null;
  }
  if (!(workBuffer[GWGETUsed])) {
    /* currentYPut: */
    workBuffer[GWCurrentY] = (workBuffer[GWFillMaxY]);
    return 0;
  }

  /* Nothing to do
     Sort entries in the GET */

  /* begin sortGlobalEdgeTable */
  quickSortGlobalEdgeTablefromto(getBuffer, 0, (workBuffer[GWGETUsed]) - 1);

  /* Find the first y value to be processed */
  value = objBuffer[(getBuffer[0]) + GEYValue];

  /* begin currentYPut: */
  workBuffer[GWCurrentY] = value;
  if ((workBuffer[GWCurrentY]) < (workBuffer[GWFillMinY])) {
    /* currentYPut: */
    workBuffer[GWCurrentY] = (workBuffer[GWFillMinY]);
  }

  /* Load and clear the span buffer */

  /* spanStartPut: */
  workBuffer[GWSpanStart] = 0;

  /* spanEndPut: */
  workBuffer[GWSpanEnd] =
      (((((usqInt)((workBuffer[GWSpanSize])) << (workBuffer[GWAAShift])))) - 1);
  clearSpanBuffer();
  return 0;
}