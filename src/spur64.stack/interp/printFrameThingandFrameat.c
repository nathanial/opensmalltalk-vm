/* Extracted from interp.c:59784 (function printFrameThingandFrameat). */

/* StackInterpreter>>#printFrameThing:andFrame:at: */

static NoDbgRegParms void printFrameThingandFrameat(char *name, char *theFP,
                                                    char *address) {
  sqInt it;

  it = longAt(address);

  /* begin printFrameAddress: */
  fprintf(transcript, "%16p:", ((void *)address));
  fprintf(transcript, "%12s: %16p" /* pst: */, name, ((void *)it));
  if (it) {
    if (it == nilObj) {
      fprintf(transcript, "=nil");
    } else {
      fprintf(transcript, "=%" PRIdSQINT "", it);
    }
  }
  fprintf(transcript, " frame: %p\n", theFP);
}