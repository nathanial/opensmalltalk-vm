/* Extracted from B2DPlugin.c:3732 (function insertEdgeIntoAET). */

static sqInt insertEdgeIntoAET(sqInt edge) {
  sqInt index;

  /* Check for the number of lines remaining */
  if ((objBuffer[edge + GENumLines]) <= 0) {
    return null;
  }
  index = indexForInsertingIntoAET(edge);

  /* And insert edge */
  insertToAETbeforeIndex(edge, index);
  return 0;
}