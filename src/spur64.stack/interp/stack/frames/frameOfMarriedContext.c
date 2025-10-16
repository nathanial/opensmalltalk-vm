/* Extracted from interp.c:54009 (function frameOfMarriedContext). */

/* StackInterpreter>>#frameOfMarriedContext: */

static char *frameOfMarriedContext(sqInt aContext) {
  sqInt senderOop;

  senderOop = fetchPointerofObject(SenderIndex, aContext);

  /* begin withoutSmallIntegerTags: */
  assert((((senderOop) & 7) == 1));
  return ((char *)(senderOop - (smallIntegerTag())));
}