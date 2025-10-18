/* Extracted from interp.c:53004 (function fetchPointerofMarriedContext). */

/*	Fetch a slot from a married context. Compute the value
        of the relevant inst var from the spouse frame's state.

        This method assumes frame pointers have been written back. */

/* StackInterpreter>>#fetchPointer:ofMarriedContext: */

static sqInt fetchPointerofMarriedContext(sqInt offset, sqInt aContext) {
  char *callerFP;
  usqInt frameNumArgs;
  sqInt senderOop;
  char *spouseFP;

  assert(isContext(aContext));
  assert((((stackPage->headFP)) == framePointer) &&
         (((stackPage->headSP)) == stackPointer));
  assert(checkIsStillMarriedContextcurrentFP(aContext, framePointer));

  /* method, closureOrNil & receiver need no special handling; only
     sender, pc & stackp have to be computed for married contexts. */
  if (offset <= ReceiverIndex) {
    if (!(offset <= StackPointerIndex)) {
      return fetchPointerofObject(offset, aContext);
    }

    /* begin frameOfMarriedContext: */
    senderOop = fetchPointerofObject(SenderIndex, aContext);
    assert((((senderOop) & 7) == 1));
    spouseFP = ((char *)(senderOop - (smallIntegerTag())));
    if (!offset) {
      /* begin ensureCallerContext: */
      callerFP = ((char *)(longAt(spouseFP + FoxSavedFP)));
      if (!callerFP) {
        /* begin frameCallerContext: */
        assert(isBaseFrame(spouseFP));
        return longAt(spouseFP + FoxCallerContext);
      }

      /* base frame, context in saved ip slot (or base of stack in Cog) */

      /* begin ensureFrameIsMarried:SP: */
      if (byteAt((callerFP + FoxFrameFlags) + 2)) {
        assert(isContext(frameContext(callerFP)));
        return longAt(callerFP + FoxThisContext);
      }
      return marryFrameSP(
          callerFP,
          (/* begin frameCallerStackPointer: */
           assert(!(isBaseFrame(spouseFP))),
           (spouseFP + ((FoxCallerSavedIP + BytesPerWord) +
                        ((((usqInt)((byteAt((spouseFP + FoxFrameFlags) + 1)))
                           << (shiftForWord())))))) +
               BytesPerWord));
    }
    if (offset == StackPointerIndex) {
      return (((stackPointerIndexForFrame(spouseFP)) << 3) | 1);
    }
    if (offset == InstructionPointerIndex) {
      return instructionPointerForFramecurrentFPcurrentIP(
          spouseFP, framePointer, instructionPointer);
    }
  }

  /* begin frameOfMarriedContext: */
  senderOop = fetchPointerofObject(SenderIndex, aContext);
  assert((((senderOop) & 7) == 1));
  spouseFP = ((char *)(senderOop - (smallIntegerTag())));
  return ((((offset - ReceiverIndex) >= 1) &&
           ((offset - ReceiverIndex) <= (stackPointerIndexForFrame(spouseFP))))
              ? temporaryin((offset - (ReceiverIndex + 1)), spouseFP)
              : nilObj);
}
