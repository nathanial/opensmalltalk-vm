/* Extracted from interp.c:59679 (function printFrameAndCallersSPshort). */

/* StackInterpreter>>#printFrameAndCallers:SP:short: */

static sqInt printFrameAndCallersSPshort(char *theFP, char *theSP,
                                         sqInt printShort) {
  if (!(/* couldBeFramePointer: */
        (((((usqInt)theFP)) & (BytesPerWord - 1)) == 0) &&
        ((((((usqInt)theFP)) >= (((usqInt)stackMemory))) &&
          ((((usqInt)theFP)) <= (((usqInt)pages))))))) {
    return null;
  }
  if (longAt(theFP + FoxSavedFP)) {
    printFrameAndCallersSPshort(((char *)(longAt(theFP + FoxSavedFP))),
                                frameCallerSP(theFP), printShort);
  }
  if (printShort) {
    shortPrintFrame(theFP);
    return null;
  }
  cr();
  printFrameWithSP(theFP, theSP);
  return 0;
}