/* Extracted from interp.c:11954 (function
 * primitiveArrayBecomeOneWayCopyHashArg). */

/*	Similar to primitiveArrayBecomeOneWay but accepts a third argument
        deciding whether to
        copy the receiver's elements identity hashes over the argument's
   elements identity hashes. See the comment in primitiveArrayBecome for
   handling pervasive effects on method cacheing and jitting.
 */

/* InterpreterPrimitives>>#primitiveArrayBecomeOneWayCopyHashArg */

static void primitiveArrayBecomeOneWayCopyHashArg(void) {
  sqInt copyHashFlag;
  sqInt ec;

  copyHashFlag = 0;
  if ((longAt(stackPointer)) == trueObj) {
    copyHashFlag = 1;
  } else {
    if ((longAt(stackPointer)) == falseObj) {
      copyHashFlag = 0;
    } else {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      return;
    }
  }
  ec = becomewithtwoWaycopyHash(longAt(stackPointer + (2 * BytesPerWord)),
                                longAt(stackPointer + (1 * BytesPerWord)), 0,
                                copyHashFlag);

  /* begin primitiveBecomeReturn: */
  if (ec) {
    /* primitiveFailFor: */
    primFailCode = ec;
  } else {
    /* begin methodReturnReceiver */
    assert(!((failed())));
    stackPointer += argumentCount * BytesPerWord;
  }
}