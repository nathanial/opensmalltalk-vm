/* Extracted from interp.c:63240 (function shortPrintFrame). */

/* StackInterpreter>>#shortPrintFrame: */

static sqInt shortPrintFrame(char *theFP) {
  usqInt frameNumArgs;
  sqInt rcvr;

  if (!(/* couldBeFramePointer: */
        (((((usqInt)theFP)) & (BytesPerWord - 1)) == 0) &&
        ((((((usqInt)theFP)) >= (((usqInt)stackMemory))) &&
          ((((usqInt)theFP)) <= (((usqInt)pages))))))) {
    /* begin print: */
    fprintf(transcript, "%s", "invalid frame pointer");
    cr();
    return null;
  }
  rcvr = longAt(theFP + FoxReceiver);
  printHexPtrnp(theFP);

  /* begin space */
  printChar(' ');
  printActivationNameForreceiverisBlockfirstTemporary(
      longAt(theFP + FoxMethod), rcvr,
      (byteAt((theFP + FoxFrameFlags) + 3)) != 0, temporaryin(0, theFP));

  /* begin space */
  printChar(' ');
  shortPrintOop(rcvr);
  return 0;
}
