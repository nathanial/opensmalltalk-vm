/* Extracted from interp.c:63119 (function shortPrintContext). */

/* StackInterpreter>>#shortPrintContext: */

static sqInt shortPrintContext(sqInt aContext) {
  sqInt home;
  sqInt rcvr;
  sqInt senderOop;
  char *theFP;

  if (!(/* isContext: */
        ((!(aContext & (tagMask())))) &&
        (((longAt((void *)(aContext))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex))) {
    printHex(aContext);

    /* begin print: */
    fprintf(transcript, "%s", " is not a context");
    cr();
    return null;
  }
  printHex(aContext);
  if (((((fetchPointerofObject(SenderIndex, aContext))) & 7) == 1)) {
    if (checkIsStillMarriedContextcurrentFP(aContext, framePointer)) {
      /* begin frameOfMarriedContext: */
      senderOop = fetchPointerofObject(SenderIndex, aContext);
      assert((((senderOop) & 7) == 1));
      theFP = ((char *)(senderOop - (smallIntegerTag())));

      /* begin print: */
      fprintf(transcript, "%s", " I (");
      printHex(((usqIntptr_t)theFP));

      /* begin print: */
      fprintf(transcript, "%s", ") ");
    } else {
      /* begin print: */
      fprintf(transcript, "%s", " w ");
    }
  } else {
    /* begin print: */
    fprintf(transcript, "%s", " s ");
  }
  if ((home = findHomeForContext(aContext))) {
    rcvr = fetchPointerofObject(ReceiverIndex, home);
    printActivationNameForreceiverisBlockfirstTemporary(
        fetchPointerofObject(MethodIndex, aContext), rcvr, home != aContext,
        fetchPointerofObject(0 + CtxtTempFrameStart, home));

    /* begin space */
    printChar(' ');
    shortPrintOop(rcvr);
  } else {
    /* begin print: */
    fprintf(transcript, "%s", " BOGUS CONTEXT (can't determine home)");
    cr();
  }
  return 0;
}