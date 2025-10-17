/* Extracted from interp.c:11147 (function identityHashOf). */

/*	Answer the identity hash of an object, assigning a hash if it doesn't
   have one. On Spur refuse to assign a hash to something that looks like a
        behavior.
 */

/* InterpreterPrimitives>>#identityHashOf: */

sqInt identityHashOf(sqInt anOop) {
  usqInt hash;
  usqInt hashUsqInt;

  if (((anOop & (tagMask())) != 0)) {
    /* begin immediateAsInteger:ifFail: */
    if ((((anOop) & 7) == 1)) {
      return ((usqInt)((anOop >> 3)));
    }
    if (((anOop & (characterTag())) != 0)) {
      return ((((usqInt)anOop))) >> (numTagBits());
    }
    if (((anOop & (smallFloatTag())) != 0)) {
      /* begin rotatedFloatBitsOf: */
      assert(isImmediateFloat(anOop));
      return (((((usqInt)anOop))) >> ((numTagBits()) + 1)) +
             ((((anOop & (1U << (numTagBits()))) != 0)
                   ? ((((usqInt)-1)) << ((64 - (numTagBits())) - 1))
                   : 0));
    }
    return ((usqInt)-1);
  }
  if ((((long32At((void *)(anOop + 4))) & (identityHashHalfWordMask())) == 0) &&
      (objCouldBeClassObj(anOop))) {
    return -1;
  }

  /* begin hashBitsOf: */
  hash = (long32At((void *)(anOop + 4))) & (identityHashHalfWordMask());
  if (!hash) {
    /* begin newHashBitsOf: */
    /* begin newObjectHash */
    assert(!((lastHash == 0)));
    lastHash = lastHash ^ (((lastHash) >> 2));
    lastHash = lastHash ^ ((((lastHash << 7))) & (identityHashHalfWordMask()));
    lastHash = lastHash ^ (((lastHash) >> 3));
    hashUsqInt = lastHash;

    /* begin setHashBitsOf:to: */
    long32Atput(
        (void *)(anOop + 4),
        ((((long32At((void *)(anOop + 4))) | (identityHashHalfWordMask())) -
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
  return hash;
}