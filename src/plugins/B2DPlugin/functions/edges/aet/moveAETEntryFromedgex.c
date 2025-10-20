/* Extracted from B2DPlugin.c:5617 (function moveAETEntryFromedgex). */

static sqInt moveAETEntryFromedgex(sqInt index, sqInt edge, sqInt xValue) {
  sqInt newIndex;

  newIndex = index;
  while ((newIndex > 0) &&
         ((objBuffer[(aetBuffer[newIndex - 1]) + GEXValue]) > xValue)) {
    aetBuffer[newIndex] = (aetBuffer[newIndex - 1]);
    newIndex -= 1;
  }
  aetBuffer[newIndex] = edge;
  return 0;
}