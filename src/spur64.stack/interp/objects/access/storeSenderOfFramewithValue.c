/* Extracted from interp.c:64843 (function storeSenderOfFramewithValue). */

/*	Set the sender of a frame. If the frame is a base frame then this is
        trivial; merely store into the FoxCallerSavedIP/FoxCallerContext field.
   If not, then split the stack at the frame, moving the frame and those hotter
   than it to a new stack page. In the new stack page the frame will be the base
   frame and storing trivial. Answer the possibly changed location of theFP. */

/* StackInterpreter>>#storeSenderOfFrame:withValue: */

static char *storeSenderOfFramewithValue(char *theFP, sqInt anOop) {
  char *callerFP;
  StackPage *newPage;
  int onCurrentPage;
  char *theMovedFP;
  StackPage *thePage;

  if (!(longAt(theFP + FoxSavedFP))) {
    /* begin frameCallerContext:put: */
    assert(isBaseFrame(theFP));
    assert((anOop == (nilObject())) || (isContext(anOop)));
    longAtput(theFP + FoxCallerContext, anOop);
    return theFP;
  }

  /* begin ensureCallerContext: */
  callerFP = ((char *)(longAt(theFP + FoxSavedFP)));
  if (!callerFP) {
    /* begin frameCallerContext: */
    assert(isBaseFrame(theFP));
    goto l2;
  }

  /* base frame, context in saved ip slot (or base of stack in Cog) */

  /* begin ensureFrameIsMarried:SP: */
  if (byteAt((callerFP + FoxFrameFlags) + 2)) {
    assert(isContext(frameContext(callerFP)));

    /* frameContext: */
    goto l2;
  }
  marryFrameSP(callerFP,
               (/* begin frameCallerStackPointer: */
                assert(!(isBaseFrame(theFP))),
                (theFP + ((FoxCallerSavedIP + BytesPerWord) +
                          ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1)))
                             << (shiftForWord())))))) +
                    BytesPerWord));
  /* end ensureCallerContext: */
l2:

  /* begin stackPageFor: */
  thePage = stackPageAtpages(
      pageIndexForstackMemorybytesPerPage(theFP, stackMemory, bytesPerPage),
      pages);
  assert(stackPage == (mostRecentlyUsedPage));
  onCurrentPage = thePage == stackPage;
  if (!onCurrentPage) {
    markStackPageNextMostRecentlyUsed(thePage);
  }

  /* Make sure the frame's page isn't divorced when a new page is allocated. */

  /* begin newStackPage */
  newPage = (mostRecentlyUsedPage->nextPage);
  if (!((newPage->baseFP))) {
    goto l1;
  }
  divorceFramesIn(newPage);
  /* end newStackPage */
l1:
  theMovedFP = moveFramesInthroughtoPage(thePage, theFP, newPage);
  if (onCurrentPage) {
    /* begin setStackPageAndLimit: */
    assert(newPage);
    stackPage = newPage;
    if (stackLimit != (((char *)(((usqInt)-1))))) {
      stackLimit = (stackPage->stackLimit);
    }
    markStackPageMostRecentlyUsed(newPage);
  } else {
    markStackPageMostRecentlyUsed(newPage);
  }
  assert(isBaseFrame(theMovedFP));

  /* begin frameCallerContext:put: */
  assert(isBaseFrame(theMovedFP));
  assert((anOop == (nilObject())) || (isContext(anOop)));
  longAtput(theMovedFP + FoxCallerContext, anOop);
  return theMovedFP;
}