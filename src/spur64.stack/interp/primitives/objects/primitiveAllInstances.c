/* Extracted from interp.c:11757 (function primitiveAllInstances). */

/*	Answer an array of all instances of the receiver that exist
        when the primitive is called, excluding any that may be
        garbage collected as a side effect of allocating the result array. */

/* InterpreterPrimitives>>#primitiveAllInstances */

EXPORT(void)
primitiveAllInstances(void) {
  sqInt delta;
  sqInt result;
  char *sp;

  /* For the mirror prims check that the class obj is actually a valid class. */
  result = allInstancesOf(longAt(stackPointer));
  if ((((result) & 7) == 1)) {
    /* begin growToAccomodateContainerWithNumSlots: */
    delta = (BaseHeaderSize * 2) + (((result >> 3)) * BytesPerOop);
    growOldSpaceByAtLeast(((growHeadroom < delta) ? delta : growHeadroom));
    result = allInstancesOf(longAt(stackPointer));
    if ((((result) & 7) == 1)) {
      /* primitiveFailFor: */
      primFailCode = PrimErrNoMemory;
      return;
    }
  }

  /* begin pop:thenPush: */
  popthenPush(argumentCount + 1, result);
}