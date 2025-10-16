/* Extracted from B2DPlugin.c:770 (function allocateGETEntry). */

static sqInt allocateGETEntry(sqInt nSlots) {
  sqInt dstIndex;
  sqInt i;
  int srcIndex;

  /* First allocate nSlots in the AET */
  /* begin allocateAETEntry: */
  /* begin needAvailableSpace: */
  if (((((GWHeaderSize + objUsed) + (workBuffer[GWGETUsed])) +
        (workBuffer[GWAETUsed])) +
       nSlots) > (workBuffer[GWBufferTop])) {
    /* begin stopBecauseOf: */
    /* stopReasonPut: */
    workBuffer[GWStopReason] = GErrorNoMoreSpace;
    engineStopped = 1;
    return 0;
  }
  if (workBuffer[GWAETUsed]) {
    srcIndex = workBuffer[GWAETUsed];
    dstIndex = (workBuffer[GWAETUsed]) + nSlots;
    for (i = 1; i <= (workBuffer[GWAETUsed]); i += 1) {
      aetBuffer[(dstIndex -= 1)] = (aetBuffer[(srcIndex -= 1)]);
    }
  }

  /* Then move the AET upwards */
  aetBuffer += nSlots;
  return 1;
}