/* Extracted from interp.c:59815 (function printFrameThingatextraString). */

/* StackInterpreter>>#printFrameThing:at:extraString: */

static NoDbgRegParms void printFrameThingatextraString(char *name,
                                                       char *address,
                                                       char *extraStringOrNil) {
  sqInt it;

  it = longAt(address);

  /* begin printFrameAddress: */
  fprintf(transcript, "%16p:", ((void *)address));
  fprintf(transcript, "%12s: %16p" /* pst: */, name, ((void *)it));
  framePrintDescription(it);
  if (extraStringOrNil) {
    fprintf(transcript, "%s", extraStringOrNil);
  }
  cr();
}