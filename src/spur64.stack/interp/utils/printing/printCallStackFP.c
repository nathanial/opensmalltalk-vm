/* Extracted from interp.c:59430 (function printCallStackFP). */

/* StackInterpreter>>#printCallStackFP: */

static sqInt printCallStackFP(char *theFP) {
  sqInt context;

  context = shortReversePrintFrameAndCallers(theFP);
  while (!(context == nilObj)) {
    if (((((fetchPointerofObject(SenderIndex, context))) &
          7) == 1)) {
      if (!(checkIsStillMarriedContextcurrentFP(context, framePointer))) {
        shortPrintContext(context);
        return null;
      }
      context =
          shortReversePrintFrameAndCallers(frameOfMarriedContext(context));
    } else {
      context = printContextCallStackOf(context);
    }
  }
  return 0;
}