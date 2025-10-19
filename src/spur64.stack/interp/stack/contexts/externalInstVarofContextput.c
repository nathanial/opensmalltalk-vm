/* Extracted from interp.c:52776 (function externalInstVarofContextput). */

/* StackInterpreter>>#externalInstVar:ofContext:put: */

static sqInt externalInstVarofContextput(sqInt index, sqInt maybeMarriedContext,
                                         sqInt anOop) {
  int onCurrentPage;
  sqInt senderOop;
  char *theFP;
  StackPage *thePage;

  assert(isContext(maybeMarriedContext));

  externalWriteBackHeadFramePointers();

  /* Assign the field of a married context. */
  assert(!((isObjImmutable(maybeMarriedContext))));
  if (!(/* isStillMarriedContext: */
        (((((fetchPointerofObject(SenderIndex, maybeMarriedContext))) & 7) ==
          1)) &&
        (!(isWidowedContext(maybeMarriedContext))))) {
    storePointerofObjectwithValue(index, maybeMarriedContext, anOop);
    return null;
  }

  /* begin frameOfMarriedContext: */
  senderOop = fetchPointerofObject(SenderIndex, maybeMarriedContext);
  assert((((senderOop) & 7) == 1));
  theFP = ((char *)(senderOop - (smallIntegerTag())));

  /* begin stackPageFor: */
  thePage = stackPageAtpages(
      pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage),
      pages);
  assert(stackPage == (mostRecentlyUsedPage));
  onCurrentPage = thePage == stackPage;
  if (index) {
    externalDivorceFrameandContext(theFP, maybeMarriedContext);

    storePointerofObjectwithValue(index, maybeMarriedContext, anOop);
  } else {
    storeSenderOfFramewithValue(theFP, anOop);
  }
  if (onCurrentPage) {
    /* begin setStackPointersFromPage: */
    stackPointer = (stackPage->headSP);
    framePointer = (stackPage->headFP);
  } else {
    markStackPageMostRecentlyUsed(stackPage);
  }
  assert(stackPage == (mostRecentlyUsedPage));
  assert(pageListIsWellFormed());
  assert(validStackPageBaseFrames());
  return 0;
}