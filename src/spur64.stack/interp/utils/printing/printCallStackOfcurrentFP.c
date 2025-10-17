/* Extracted from interp.c:59488 (function printCallStackOfcurrentFP). */

/* StackInterpreter>>#printCallStackOf:currentFP: */

static sqInt printCallStackOfcurrentFP(sqInt aContext, char *currFP) {
  sqInt ctxt;
  sqInt senderOop;
  char *theFP;
  StackPage *thePage;

  ctxt = aContext;
  while (!(ctxt == nilObj)) {
    if (((((longAt(
              (void *)((ctxt + BaseHeaderSize) +
                       ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
          7) == 1)) {
      /* begin frameOfMarriedContext: */
      senderOop =
          longAt((void *)((ctxt + BaseHeaderSize) +
                          ((((usqInt)(SenderIndex) << (shiftForWord()))))));
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
      ctxt = longAt((void *)((ctxt + BaseHeaderSize) +
                             ((((usqInt)(SenderIndex) << (shiftForWord()))))));
    }
  }
  return 0;
}