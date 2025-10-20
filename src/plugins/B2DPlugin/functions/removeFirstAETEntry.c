/* Extracted from B2DPlugin.c:9503 (function removeFirstAETEntry). */

static sqInt removeFirstAETEntry(void) {
  int index;

  index = workBuffer[GWAETStart];

  /* aetUsedPut: */
  workBuffer[GWAETUsed] = ((workBuffer[GWAETUsed]) - 1);
  while (index < (workBuffer[GWAETUsed])) {
    aetBuffer[index] = (aetBuffer[index + 1]);
    index += 1;
  }
  return 0;
}