/* Extracted from interp.c:55304 (function isWidowedContextNoConvert). */

/*	Answer if the argument is married to a live frame or not.
        This method is safe for use only when no frameContext fields may be
        forwarded (as maybe the case when scavenging). Post become: all
        frameContext fields are followed, and hence normally no following of
        frameContext fields is necessary. */
/*	i.e. inline into isWidowedContext: */

/* StackInterpreter>>#isWidowedContextNoConvert: */

static NoDbgRegParms sqInt
isWidowedContextNoConvert(sqInt aOnceMarriedContext) {
  sqInt anInteger;
  sqInt senderOop;
  char *shouldBeFrameCallerField;
  char *theFrame;
  StackPage *thePage;

  assert((isContext(aOnceMarriedContext)) &&
         (isMarriedOrWidowedContext(aOnceMarriedContext)));

  /* begin frameOfMarriedContext: */
  senderOop = longAt((void *)((aOnceMarriedContext + BaseHeaderSize) +
                              ((((usqInt)(SenderIndex) << (shiftForWord()))))));
  assert((((senderOop) & 7) == 1));
  theFrame = ((char *)(senderOop - (smallIntegerTag())));

  /* begin stackPageFor: */
  thePage = stackPageAtpages(
      pageIndexForstackMemorybytesPerPage(theFrame, stackMemory, bytesPerPage),
      pages);
  if (!((isFree(thePage)) || (theFrame < ((thePage->headFP))))) {
    anInteger = longAt(
        (void *)((aOnceMarriedContext + BaseHeaderSize) +
                 ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))));

    /* begin withoutSmallIntegerTags: */
    assert((((anInteger) & 7) == 1));
    shouldBeFrameCallerField = ((char *)(anInteger - (smallIntegerTag())));
    if (((((char *)(longAt(theFrame + FoxSavedFP)))) ==
         shouldBeFrameCallerField) &&
        ((byteAt((theFrame + FoxFrameFlags) + 2)) != 0)) {
      assert(!(((isFrameonPage(theFrame, thePage)) &&
                (isForwarded(frameContext(theFrame))))));
      if ((longAt(theFrame + FoxThisContext)) == aOnceMarriedContext) {
        return 0;
      }
    }
  }

  /* The frame pointer is within the bounds of a live page.
     Now check if it matches a frame. */
  return 1;
}