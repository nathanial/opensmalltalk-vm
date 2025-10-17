/* Extracted from interp.c:59488 (function printCallStackOfcurrentFP). */

/* StackInterpreter>>#printCallStackOf:currentFP: */

static sqInt printCallStackOfcurrentFP(sqInt aContext, char *currFP) {
  sqInt ctxt;
  sqInt senderOop;
  char *theFP;
  StackPage *thePage;

  ctxt = aContext;
  while (!(ctxt == nilObj)) {
    if (((((fetchPointerofObject(SenderIndex, ctxt))) &
          7) == 1)) {
      /* begin frameOfMarriedContext: */
      senderOop =
          fetchPointerofObject(SenderIndex, ctxt);
      assert((((senderOop) & 7) == 1));
      theFP = ((char *)(senderOop - (smallIntegerTag())));
      if (checkIsStillMarriedContextcurrentFP(ctxt, currFP)) {
        /* begin stackPageFor: */
        thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                       theFP, stackMemory, bytesPerPage),
                                   pages);
        if (isFree(thePage)) {
          fprintf(transcript, "%p is on a free page?!\n", theFP);
          return null;
        }
        shortPrintFrameAndCallers(theFP);
        theFP = (thePage->baseFP);

        /* begin frameCallerContext: */
        assert(isBaseFrame(theFP));
        ctxt = longAt(theFP + FoxCallerContext);
        if ((!((longAt((void *)(ctxt))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          ctxt = followForwarded(ctxt);
        }
      } else {
        fprintf(transcript, "widowed caller frame %p\n", theFP);
        return null;
      }
    } else {
      shortPrintContext(ctxt);
      ctxt = fetchPointerofObject(SenderIndex, ctxt);
    }
  }
  return 0;
}