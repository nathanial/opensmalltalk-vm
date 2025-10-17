/* Extracted from interp.c:55112 (function isMarriedOrWidowedContext). */

/* StackInterpreter>>#isMarriedOrWidowedContext: */

static NoDbgRegParms int isMarriedOrWidowedContext(sqInt aContext) {
  return (
      (((longAt((void *)((aContext + BaseHeaderSize) +
                         ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
       7) == 1);
}