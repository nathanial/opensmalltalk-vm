/* Extracted from interp.c:59560 (function printContextCallStackOf). */

/*	Print the call stack of aContext until it links to a frame. */

/* StackInterpreter>>#printContextCallStackOf: */

static sqInt printContextCallStackOf(sqInt aContext) {
  sqInt ctxt;

  ctxt = aContext;
  while (!(
      (ctxt == nilObj) ||
      (((((longAt((void *)((ctxt + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
         7) == 1)))) {
    shortPrintContext(ctxt);
    ctxt = longAt((void *)((ctxt + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord()))))));
  }
  return ctxt;
}