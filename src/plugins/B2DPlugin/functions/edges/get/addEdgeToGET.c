/* Extracted from B2DPlugin.c:622 (function addEdgeToGET). */

static sqInt addEdgeToGET(sqInt edge) {
  if (!(allocateGETEntry(1))) {
    return 0;
  }

  /* Install edge in the GET */
  getBuffer[workBuffer[GWGETUsed]] = edge;

  /* getUsedPut: */
  workBuffer[GWGETUsed] = ((workBuffer[GWGETUsed]) + 1);
  return 0;
}