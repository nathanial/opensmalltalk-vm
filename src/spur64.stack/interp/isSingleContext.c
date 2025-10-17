/* Extracted from interp.c:55175 (function isSingleContext). */

/* StackInterpreter>>#isSingleContext: */

static NoDbgRegParms int isSingleContext(sqInt aContext) {
  return (
      !((longAt((void *)((aContext + BaseHeaderSize) +
                         ((((usqInt)(SenderIndex) << (shiftForWord()))))))) &
        (tagMask())));
}