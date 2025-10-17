/* Extracted from interp.c:54009 (function frameOfMarriedContext). */

/* StackInterpreter>>#frameOfMarriedContext: */

static NoDbgRegParms char *frameOfMarriedContext(sqInt aContext) {
  sqInt senderOop;

  senderOop = longAt((void *)((aContext + BaseHeaderSize) +
                              ((((usqInt)(SenderIndex) << (shiftForWord()))))));

  /* begin withoutSmallIntegerTags: */
  assert((((senderOop) & 7) == 1));
  return ((char *)(senderOop - (smallIntegerTag())));
}