/* Extracted from B2DPlugin.c:803 (function allocateObjEntry). */

static sqInt allocateObjEntry(sqInt nSlots) {
  sqInt dstIndex;
  sqInt i;
  int srcIndex;

  /* First allocate nSlots in the GET */
  if (!(allocateGETEntry(nSlots))) {
    return 0;
  }
  if (workBuffer[GWGETUsed]) {
    srcIndex = workBuffer[GWGETUsed];
    dstIndex = (workBuffer[GWGETUsed]) + nSlots;
    for (i = 1; i <= (workBuffer[GWGETUsed]); i += 1) {
      getBuffer[(dstIndex -= 1)] = (getBuffer[(srcIndex -= 1)]);
    }
  }

  /* Then move the GET upwards */
  getBuffer += nSlots;
  return 1;
}