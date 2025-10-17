/* Extracted from interp.c:17764 (function primitiveIdentityHash). */

/* InterpreterPrimitives>>#primitiveIdentityHash */

static void primitiveIdentityHash(void) {
  usqInt hash;
  usqInt hashUsqInt;
  sqInt integer;
  char *sp;
  sqInt thisReceiver;

  thisReceiver = longAt(stackPointer);
  if ((((thisReceiver & (tagMask())) != 0)) ||
      ((argumentCount > 0) &&
       ((!((longAt((void *)(thisReceiver))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
    /* primitiveFailFor: */
    primFailCode =
        (argumentCount > 0 ? PrimErrBadArgument : PrimErrBadReceiver);
    return;
  }

  /* fail if forwarded; the retry machinery will fix things up. */

  /* begin hashBitsOf: */
  hash = (long32At((void *)(thisReceiver + 4))) & (identityHashHalfWordMask());
  if (!hash) {
    /* begin newHashBitsOf: */
    /* begin newObjectHash */
    assert(!((lastHash == 0)));
    lastHash = lastHash ^ (((lastHash) >> 2));
    lastHash = lastHash ^ ((((lastHash << 7))) & (identityHashHalfWordMask()));
    lastHash = lastHash ^ (((lastHash) >> 3));
    hashUsqInt = lastHash;

    /* begin setHashBitsOf:to: */
    long32Atput((void *)(thisReceiver + 4),
                ((((long32At((void *)(thisReceiver + 4))) |
                   (identityHashHalfWordMask())) -
                  (identityHashHalfWordMask()))) +
                    (hashUsqInt & (identityHashHalfWordMask())));
    hash = hashUsqInt & (identityHashHalfWordMask());
  }

  /* cb 1/19/2017 18:34:
     would like to assert
     self assert: (coInterpreter addressCouldBeClassObj: objOop) not
     but instance-specific behaviors that are instances of themselves may
     fail this test.
     eem 12/28/2021 11:52 I'm not sure this is the issue. The issue is only
     validating objOop as a behavior. */
  integer = hash;

  /* begin methodReturnInteger: */
  assert(!((failed())));
  longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
            (((usqInt)integer << 3) | 1));
  stackPointer = sp;
}