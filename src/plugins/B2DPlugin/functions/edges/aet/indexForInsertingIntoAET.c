/* Extracted from B2DPlugin.c:3574 (function indexForInsertingIntoAET). */

static sqInt indexForInsertingIntoAET(sqInt edge) {
  sqInt index;
  int initialX;

  initialX = objBuffer[edge + GEXValue];
  index = 0;
  while ((index < (workBuffer[GWAETUsed])) &&
         ((objBuffer[(aetBuffer[index]) + GEXValue]) < initialX)) {
    index += 1;
  }
  while ((index < (workBuffer[GWAETUsed])) &&
         (((objBuffer[(aetBuffer[index]) + GEXValue]) == initialX) &&
          (getSortsbefore(aetBuffer[index], edge)))) {
    index += 1;
  }
  return index;
}