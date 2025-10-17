/* Extracted from interp.c:51334 (function
 * checkStackPointerForMaybeMarriedContext). */

/*	Version of stackPointerForMaybeMarriedContext: with no side-effects
   (does not widow). Used for assertion checking. Safe only in external
   primitives (framePointer valid). Answer the stackPointer of a Context. */

/* StackInterpreter>>#checkStackPointerForMaybeMarriedContext: */

static sqInt checkStackPointerForMaybeMarriedContext(sqInt aContext) {
  sqInt header;
  sqInt methodPointer;
  sqInt sp;
  sqInt spSqInt;

  if (checkIsStillMarriedContextcurrentFP(aContext, framePointer)) {
    sp = checkStackPointerIndexForFrame(frameOfMarriedContext(aContext));
    assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(aContext)));
    return sp;
  }
  if (((((fetchPointerofObject(SenderIndex, aContext))) &
        7) == 1)) {
    methodPointer =
        fetchPointerofObject(MethodIndex, aContext);

    /* begin argumentCountOf: */
    /* begin methodHeaderOf: */
    assert(isCompiledMethod(methodPointer));
    header = fetchPointerofObject(HeaderIndex, methodPointer);

    /* begin argumentCountOfMethodHeader: */
    return (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;
  }
  spSqInt =
      fetchPointerofObject(StackPointerIndex, aContext);
  if (!((((spSqInt) & 7) == 1))) {
    return 0;
  }
  assert((ReceiverIndex + ((spSqInt >> 3))) < (lengthOf(aContext)));
  return (spSqInt >> 3);
}