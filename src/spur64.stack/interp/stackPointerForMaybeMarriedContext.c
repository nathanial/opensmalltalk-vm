/* Extracted from interp.c:64370 (function stackPointerForMaybeMarriedContext).
 */

/*	Return the stackPointer of a Context or BlockContext. */

/* StackInterpreter>>#stackPointerForMaybeMarriedContext: */

static sqInt stackPointerForMaybeMarriedContext(sqInt aContext) {
  usqInt sp;
  sqInt spSqInt;

  if (/* isStillMarriedContext: */
      (((((longAt((void *)((aContext + BaseHeaderSize) +
                           ((((usqInt)(SenderIndex) << (shiftForWord())))))))) &
         7) == 1)) &&
      (!(isWidowedContext(aContext)))) {
    sp = stackPointerIndexForFrame(frameOfMarriedContext(aContext));
    assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(aContext)));
    return sp;
  }

  /* begin fetchStackPointerOf: */
  spSqInt =
      longAt((void *)((aContext + BaseHeaderSize) +
                      ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
  if (!((((spSqInt) & 7) == 1))) {
    return 0;
  }
  assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(aContext)));
  return (spSqInt >> 3);
}