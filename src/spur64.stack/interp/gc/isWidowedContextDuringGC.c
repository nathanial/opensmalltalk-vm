/* Extracted from interp.c:55219 (function isWidowedContextDuringGC). */

/*	See if the argument is married to a live frame or not. i.e. see if there
        is a matching
        frame whose frameContext field is aOnceMarriedContext, or a forwarder to
        it. If aOnceMarriedContext is not married to a live frame, turn it into
   a bereaved single context. This version is for use during scavenging when
   stack references may be forwarded. Following what appear to be references to
   forwarded objects on the stack is dangerous; an instruction ponter may be
   correctly aligned and may point to bytes that just happen to look like a
   forwarder. So it is only safe to follow fields that we know are frameContext
        fields; hence the stack page is walked to check that aOnceMarriedContext
        is pointing to
        a live frame. This only has to happen during scavenging because after a
        become: all
        frameContext fields have been followed and so there is no need to follow
        forwarders.  */

/* StackInterpreter>>#isWidowedContextDuringGC: */

static sqInt isWidowedContextDuringGC(sqInt aOnceMarriedContext) {
  sqInt anInteger;
  char *maybeFrame;
  sqInt maybeFrameCtxt;
  sqInt referent;
  sqInt senderOop;
  char *shouldBeFrameCallerField;
  StackPage *thePage;

  assert((isContext(aOnceMarriedContext)) &&
         (isMarriedOrWidowedContext(aOnceMarriedContext)));

  /* begin frameOfMarriedContext: */
  senderOop = fetchPointerofObject(SenderIndex, aOnceMarriedContext);
  assert((((senderOop) & 7) == 1));
  maybeFrame = ((char *)(senderOop - (smallIntegerTag())));

  /* begin stackPageFor: */
  thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(
                                 maybeFrame, stackMemory, bytesPerPage),
                             pages);
  if (!((isFree(thePage)) || (maybeFrame < ((thePage->headFP))))) {
    anInteger =
        fetchPointerofObject(InstructionPointerIndex, aOnceMarriedContext);

    /* begin withoutSmallIntegerTags: */
    assert((((anInteger) & 7) == 1));
    shouldBeFrameCallerField = ((char *)(anInteger - (smallIntegerTag())));
    if (((((char *)(longAt(maybeFrame + FoxSavedFP)))) ==
         shouldBeFrameCallerField) &&
        ((byteAt((maybeFrame + FoxFrameFlags) + 2)) != 0)) {
      maybeFrameCtxt = longAt(maybeFrame + FoxThisContext);

      /* On Spur we need to follow the context to check for a match, but since
         the VM is only speculating about maybeFrame being a frame, and only
         speculating about maybeContext being a context, we need to be sure
         before we can safely follow. */
      if ((isFrameonPage(maybeFrame, thePage)) &&
          ((!((longAt((void *)(maybeFrameCtxt))) &
              ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
        maybeFrameCtxt = followForwarded(maybeFrameCtxt);

        /* begin setFrameContext:to: */
        longAtput(maybeFrame + FoxThisContext, maybeFrameCtxt);
      }
      if (maybeFrameCtxt == aOnceMarriedContext) {
        return 0;
      }
    }
  }

  /* The frame pointer is within the bounds of a live page.
     Now check if it matches a frame.
     It is out of range or doesn't match the frame's context.
     It is widowed. Time to wear black. */

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