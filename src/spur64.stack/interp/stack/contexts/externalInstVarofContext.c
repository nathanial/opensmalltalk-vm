/* Extracted from interp.c:52743 (function externalInstVarofContext). */

/*	Fetch an instance variable from a maybe married context.
        If the context is still married compute the value of the
        relevant inst var from the spouse frame's state. */

/* StackInterpreter>>#externalInstVar:ofContext: */

static sqInt externalInstVarofContext(sqInt offset, sqInt aContext) {
  assert(isContext(aContext));
  assert(offset <=
         (ReceiverIndex + (checkStackPointerForMaybeMarriedContext(aContext))));

  /* method, closureOrNil & receiver need no special handling; only
     sender, pc & stackp have to be computed for married contexts. */
  if (offset <= StackPointerIndex) {
    externalWriteBackHeadFramePointers();
    if (/* isStillMarriedContext: */
        (((((fetchPointerofObject(SenderIndex, aContext))) & 7) == 1)) &&
        (!(isWidowedContext(aContext)))) {
      return fetchPointerofMarriedContext(offset, aContext);
    }
  }
  return fetchPointerofObject(offset, aContext);
}