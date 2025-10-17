/* Extracted from interp.c:52205 (function ensureCallerContext). */

/* SEND_PRINTING */

/*	Answer the caller context for a frame. If the frame has a caller
        frame that doesn't have a context, then marry the caller frame. */

/* StackInterpreter>>#ensureCallerContext: */

static NoDbgRegParms sqInt ensureCallerContext(char *theFP) {
  char *callerFP;

  callerFP = ((char *)(longAt(theFP + FoxSavedFP)));
  if (!callerFP) {
    /* begin frameCallerContext: */
    assert(isBaseFrame(theFP));
    return longAt(theFP + FoxCallerContext);
  }

  /* base frame, context in saved ip slot (or base of stack in Cog) */

  /* begin ensureFrameIsMarried:SP: */
  if (byteAt((callerFP + FoxFrameFlags) + 2)) {
    assert(isContext(frameContext(callerFP)));
    return longAt(callerFP + FoxThisContext);
  }
  return marryFrameSP(
      callerFP, (/* begin frameCallerStackPointer: */
                 assert(!(isBaseFrame(theFP))),
                 (theFP + ((FoxCallerSavedIP + BytesPerWord) +
                           ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1)))
                              << (shiftForWord())))))) +
                     BytesPerWord));
}