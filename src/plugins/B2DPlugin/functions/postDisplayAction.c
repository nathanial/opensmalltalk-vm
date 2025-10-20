/* Extracted from B2DPlugin.c:5639 (function postDisplayAction). */

static sqInt postDisplayAction(void) {
  if (((workBuffer[GWGETStart]) >= (workBuffer[GWGETUsed])) &&
      ((workBuffer[GWAETUsed]) == 0)) {
    /* statePut: */
    workBuffer[GWState] = GEStateCompleted;
  }

  /* No more entries to process */
  if ((workBuffer[GWCurrentY]) >= (workBuffer[GWFillMaxY])) {
    /* statePut: */
    workBuffer[GWState] = GEStateCompleted;
  }
  return 0;
}