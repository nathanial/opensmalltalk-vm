/* Extracted from interp.c:18695 (function primitiveIsPinned). */

/*	Answer if the receiver is pinned, i.e. immobile. */

/* InterpreterPrimitives>>#primitiveIsPinned */

static void primitiveIsPinned(void) {
  sqInt obj;
  sqInt trueOrFalse;

  obj = longAt(stackPointer);
  if ((((obj & (tagMask())) != 0)) ||
      ((!((longAt((void *)(obj))) &
          ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadReceiver;
    return;
  }
  trueOrFalse = ((byteAt((void *)(obj + (formatFieldByteOffset())))) &
                 (1U << (pinnedBitByteShift()))) != 0;

  /* begin pop:thenPushBool: */
  popthenPushBool(argumentCount + 1, /* booleanObjectOf: */
                  (trueOrFalse ? trueObj : falseObj));
}