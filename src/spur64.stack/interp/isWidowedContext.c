/* Extracted from interp.c:55355 (function isWidowedContext). */

/*	Answer if the argument is married to a live frame or not.
        If it is not, turn it into a bereaved single context. This version is
   safe for use only when no frameContext fields may be forwarded (as maybe the
   case when scavenging). Post become: all frameContext fields are followed, and
        hence normally no following of frameContext fields is necessary. But
        during a scavenge one must use isWidowedContextDuringGC:. */

/* StackInterpreter>>#isWidowedContext: */

static NoDbgRegParms sqInt isWidowedContext(sqInt aOnceMarriedContext) {
  sqInt anInteger;
  sqInt senderOop;
  char *shouldBeFrameCallerField;
  char *theFrame;
  StackPage *thePage;
  sqInt widowed;

  /* begin isWidowedContextNoConvert: */
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
        widowed = 0;
        goto l1;
      }
    }
  }

  /* The frame pointer is within the bounds of a live page.
     Now check if it matches a frame. */
  widowed = 1;
  /* end isWidowedContextNoConvert: */
l1:
  if (!widowed) {
    return 0;
  }

  /* begin markContextAsDead: */
  assert(isContext(aOnceMarriedContext));
  assert((isNonImmediate(aOnceMarriedContext)) &&
         (!(isForwarded(aOnceMarriedContext))));
  assert(
      validStorePointerUncheckedArgs(SenderIndex, aOnceMarriedContext, nilObj));
  longAtput((void *)((aOnceMarriedContext + BaseHeaderSize) +
                     ((((usqInt)(SenderIndex) << (shiftForWord()))))),
            nilObj);

  /* begin storePointerUnchecked:ofObject:withValue: */
  assert((isNonImmediate(aOnceMarriedContext)) &&
         (!(isForwarded(aOnceMarriedContext))));
  assert(validStorePointerUncheckedArgs(InstructionPointerIndex,
                                        aOnceMarriedContext, nilObj));
  longAtput(
      (void *)((aOnceMarriedContext + BaseHeaderSize) +
               ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),
      nilObj);
  return 1;
}