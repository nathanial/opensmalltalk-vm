/* Extracted from interp.c:63190 (function shortPrintFrameAndCallers). */

/* StackInterpreter>>#shortPrintFrameAndCallers: */

static sqInt shortPrintFrameAndCallers(char *theFP) {
  if (!(/* couldBeFramePointer: */
        (((((usqInt)theFP)) & (BytesPerWord - 1)) == 0) &&
        ((((((usqInt)theFP)) >= (((usqInt)stackMemory))) &&
          ((((usqInt)theFP)) <= (((usqInt)pages))))))) {
    return null;
  }
  shortPrintFrame(theFP);
  if (longAt(theFP + FoxSavedFP)) {
    shortPrintFrameAndCallers(((char *)(longAt(theFP + FoxSavedFP))));
  }
  return 0;
}