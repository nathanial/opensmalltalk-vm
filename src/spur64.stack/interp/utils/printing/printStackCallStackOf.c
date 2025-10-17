/* Extracted from interp.c:60860 (function printStackCallStackOf). */

/*	useful for VM debugging */

/* StackInterpreter>>#printStackCallStackOf: */

sqInt printStackCallStackOf(sqInt aContextOrProcessOrFrame) {
  sqInt context;
  sqInt senderOop;
  char *theFP;

  if (addressCouldBeObj(aContextOrProcessOrFrame)) {
    if ((((longAt((void *)(aContextOrProcessOrFrame))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex) &&
        (checkIsStillMarriedContextcurrentFP(aContextOrProcessOrFrame, null))) {
      return printStackCallStackOf(
          ((sqInt)(frameOfMarriedContext(aContextOrProcessOrFrame))));
    }
    if (aContextOrProcessOrFrame == (activeProcess())) {
      return printStackCallStackOf(((sqInt)framePointer));
    }
    if (couldBeProcess(aContextOrProcessOrFrame)) {
      return printCallStackOf(fetchPointerofObject(SuspendedContextIndex, aContextOrProcessOrFrame));
    }
    return null;
  }
  theFP = ((void *)aContextOrProcessOrFrame);
  while (1) {
    context = shortReversePrintFrameAndCallers(theFP);
    if (!((((((fetchPointerofObject(SenderIndex, context))) &
             7) == 1)) &&
          ((/* begin frameOfMarriedContext: */
            (senderOop = fetchPointerofObject(SenderIndex, context)),
            assert((((senderOop) & 7) == 1)),
            (theFP = ((char *)(senderOop - (smallIntegerTag())))),
            checkIsStillMarriedContextcurrentFP(context, theFP))))) {
      return null;
    }
  }
  return 0;
}