/* Extracted from interp.c:53261 (function findHomeForContext). */

/* StackInterpreter>>#findHomeForContext: */

static sqInt findHomeForContext(sqInt aContext) {
  sqInt closureOrNil;

  if (!(/* isContext: */
        ((!(aContext & (tagMask())))) &&
        (((longAt((void *)(aContext))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex))) {
    return null;
  }
  closureOrNil =
      longAt((void *)((aContext + BaseHeaderSize) +
                      ((((usqInt)(ClosureIndex) << (shiftForWord()))))));
  assert((isPointers(closureOrNil)) &&
         ((closureOrNil == (nilObject())) ||
          ((numSlotsOf(closureOrNil)) >= ClosureFirstCopiedValueIndex)));
  if ((((closureOrNil & (tagMask())) != 0)) || (closureOrNil == nilObj)) {
    return aContext;
  }

  /* robustness in the presence of bugs */
  if ((((longAt((void *)(closureOrNil))) & (classIndexMask())) !=
       ClassBlockClosureCompactIndex) &&
      (((longAt((void *)(closureOrNil))) & (classIndexMask())) !=
       ClassFullBlockClosureCompactIndex)) {
    return null;
  }
  return findHomeForContext(longAt(
      (void *)((closureOrNil + BaseHeaderSize) +
               ((((usqInt)(ClosureOuterContextIndex) << (shiftForWord())))))));
}