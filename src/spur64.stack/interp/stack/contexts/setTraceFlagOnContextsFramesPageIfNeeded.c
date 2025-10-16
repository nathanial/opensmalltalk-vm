/* Extracted from interp.c:63094 (function
 * setTraceFlagOnContextsFramesPageIfNeeded). */

/* StackInterpreter>>#setTraceFlagOnContextsFramesPageIfNeeded: */

static void setTraceFlagOnContextsFramesPageIfNeeded(sqInt aContext) {
  void *pointer;
  sqInt senderOop;
  StackPage *thePage;

  if (/* isStillMarriedContextDuringGC: */
      (((((fetchPointerofObject(SenderIndex, aContext))) & 7) == 1)) &&
      (!(isWidowedContextDuringGC(aContext)))) {
    /* begin frameOfMarriedContext: */
    senderOop = fetchPointerofObject(SenderIndex, aContext);
    assert((((senderOop) & 7) == 1));
    pointer = ((char *)(senderOop - (smallIntegerTag())));

    /* begin stackPageFor: */
    thePage = stackPageAtpages(
        pageIndexForstackMemorybytesPerPage(pointer, stackMemory, bytesPerPage),
        pages);
    assert(((((thePage->trace)) >= StackPageUnreached) &&
            (((thePage->trace)) <= StackPageTraced)));
    if (!((thePage->trace))) {
      (thePage->trace = StackPageReachedButUntraced);
    }
  }
}