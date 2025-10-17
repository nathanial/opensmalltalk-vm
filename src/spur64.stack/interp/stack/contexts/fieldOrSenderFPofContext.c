/* Extracted from interp.c:66079 (function fieldOrSenderFPofContext). */

/*	Arrange to answer naked frame pointers for unmarried
        senders to avoid reifying contexts in the search. */

/* StackInterpreterPrimitives>>#fieldOrSenderFP:ofContext: */

static sqInt fieldOrSenderFPofContext(sqInt index, sqInt contextObj) {
  sqInt senderOop;
  char *spouseFP;
  sqInt tempIndex;

  tempIndex = index - CtxtTempFrameStart;
  if (!(/* isStillMarriedContext: */
        (((((fetchPointerofObject(SenderIndex, contextObj))) & 7) == 1)) &&
        (!(isWidowedContext(contextObj))))) {
    if (tempIndex >= (fetchStackPointerOf(contextObj))) {
      return nilObj;
    } else {
      return fetchPointerofObject(index, contextObj);
    }
  }

  /* begin frameOfMarriedContext: */
  senderOop = fetchPointerofObject(SenderIndex, contextObj);
  assert((((senderOop) & 7) == 1));
  spouseFP = ((char *)(senderOop - (smallIntegerTag())));
  if (tempIndex >= (stackPointerIndexForFrame(spouseFP))) {
    return nilObj;
  }
  return fieldofFrame(index, spouseFP);
}