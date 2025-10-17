/* Extracted from interp.c:52703 (function externalEnsureIsBaseFrame). */

/*	Ensure aFramePtr is a base frame. Then we can assign its sender.
        Answer the possibly moved location of the frame. */

/* StackInterpreter>>#externalEnsureIsBaseFrame: */

static char *externalEnsureIsBaseFrame(char *aFramePtr) {
  int onCurrent;
  char *theFP;
  StackPage *thePage;

  if (!(longAt(aFramePtr + FoxSavedFP))) {
    return aFramePtr;
  }
  theFP = aFramePtr;

  /* begin stackPageFor: */
  thePage = stackPageAtpages(
      pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage),
      pages);
  onCurrent = thePage == stackPage;

  /* Storing the frame's sender with its caller's context
     has the side effect of making theFP a base frame. */
  theFP = storeSenderOfFramewithValue(theFP, ensureCallerContext(theFP));
  if (onCurrent) {
    assert(stackPage != thePage);

    /* begin setStackPointersFromPage: */
    stackPointer = (stackPage->headSP);
    framePointer = (stackPage->headFP);
  } else {
    markStackPageMostRecentlyUsed(stackPage);
  }
  assert(pageListIsWellFormed());
  assert(stackPage == (mostRecentlyUsedPage));
  return theFP;
}