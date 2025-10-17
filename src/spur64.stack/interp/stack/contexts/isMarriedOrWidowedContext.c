/* Extracted from interp.c:55112 (function isMarriedOrWidowedContext). */

/* StackInterpreter>>#isMarriedOrWidowedContext: */

static int isMarriedOrWidowedContext(sqInt aContext) {
  return (
      (((fetchPointerofObject(SenderIndex, aContext))) &
       7) == 1);
}