/* Extracted from interp.c:53947 (function frameCallerSP). */

/*	Answer the SP of the caller provided theFP is not a base frame.
        This points to the hottest item on the caller frame's stack. */

/* StackInterpreter>>#frameCallerSP: */

static char *frameCallerSP(char *theFP) {
  assert(!(isBaseFrame(theFP)));
  return (theFP + ((FoxCallerSavedIP + BytesPerWord) +
                   ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1)))
                      << (shiftForWord())))))) +
         BytesPerWord;
}