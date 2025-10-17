/* Extracted from interp.c:59455 (function printCallStackOf). */

/*	useful for VM debugging */

/* StackInterpreter>>#printCallStackOf: */

sqInt printCallStackOf(sqInt aContextOrProcessOrFrame) {
  sqInt context;

  if (/* couldBeFramePointer: */
      (((((usqInt)aContextOrProcessOrFrame)) & (BytesPerWord - 1)) == 0) &&
      ((((((usqInt)aContextOrProcessOrFrame)) >= (((usqInt)stackMemory))) &&
        ((((usqInt)aContextOrProcessOrFrame)) <= (((usqInt)pages)))))) {
    return printCallStackFP(((char *)aContextOrProcessOrFrame));
  }
  if (aContextOrProcessOrFrame == (activeProcess())) {
    return printCallStackOf(((sqInt)framePointer));
  }
  if (couldBeProcess(aContextOrProcessOrFrame)) {
    return printCallStackOf(fetchPointerofObject(SuspendedContextIndex, aContextOrProcessOrFrame));
  }
  context = aContextOrProcessOrFrame;
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