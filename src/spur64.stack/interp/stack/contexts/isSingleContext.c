/* Extracted from interp.c:55175 (function isSingleContext). */

/* StackInterpreter>>#isSingleContext: */

static int isSingleContext(sqInt aContext) {
  return (!((fetchPointerofObject(SenderIndex, aContext)) & (tagMask())));
}