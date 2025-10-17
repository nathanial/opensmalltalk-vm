/* Extracted from interp.c:54987 (function isFrameonPage). */

/* StackInterpreter>>#isFrame:onPage: */

static sqInt isFrameonPage(char *aFrame, StackPage *aPage) {
  char *prevFP;
  char *theFP;

  assert(!((isFree(aPage))));

  /* Walk the static chain making sure progress is being made,
     and in the right direction, looking for a match with aFrame. */
  theFP = (aPage->headFP);
  prevFP = theFP - BytesPerWord;
  while (1) {
    if (theFP == aFrame) {
      return 1;
    }
    if (!((theFP > prevFP) && (theFP < ((aPage->baseFP)))))
      break;
    prevFP = theFP;
    theFP = ((char *)(longAt(theFP + FoxSavedFP)));
  }
  return 0;
}