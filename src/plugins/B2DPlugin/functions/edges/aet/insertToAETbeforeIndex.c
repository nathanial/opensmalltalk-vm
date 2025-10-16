/* Extracted from B2DPlugin.c:3749 (function insertToAETbeforeIndex). */

static sqInt insertToAETbeforeIndex(sqInt edge, sqInt index) {
  int i;

  /* Make sure we have space in the AET */
  /* begin allocateAETEntry: */
  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       1) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return null;
  }
  i = (workBuffer[GWAETUsed]) - 1;
  while (!(i < index)) {
    aetBuffer[i + 1] = (aetBuffer[i]);
    i -= 1;
  }
  aetBuffer[index] = edge;

  /* aetUsedPut: */
  workBuffer[GWAETUsed] = ((workBuffer[GWAETUsed]) + 1);
  return 0;
}