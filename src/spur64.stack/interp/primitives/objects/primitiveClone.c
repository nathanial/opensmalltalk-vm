/* Extracted from interp.c:66292 (function primitiveClone). */

/*	Return a shallow copy of the receiver.
        Special-case non-single contexts (because of context-to-stack mapping).
        Can't fail for contexts cuz of image context instantiation code (sigh).
 */
/*	because of cloneContext: below */

/* StackInterpreterPrimitives>>#primitiveClone */

static void primitiveClone(void) {
  usqInt newCopy;
  sqInt rcvr;
  char *sp;

  newCopy = 0;
  rcvr = longAt(stackPointer);
  if (((rcvr & (tagMask())) != 0)) {
    newCopy = rcvr;
  } else {
    if (((longAt((void *)(rcvr))) & (classIndexMask())) ==
        ClassMethodContextCompactIndex) {
      externalWriteBackHeadFramePointers();
      newCopy = cloneContext(rcvr);
    } else {
      if ((argumentCount == 0) ||
          (!((!((longAt((void *)(rcvr))) &
                ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
        newCopy = cloneObject(rcvr);
      }
    }
    if (!newCopy) {
      /* primitiveFailFor: */
      primFailCode = PrimErrNoMemory;
      return;
    }
  }

  /* begin methodReturnValue: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            newCopy);
  stackPointer = sp;
}