/* Extracted from interp.c:17582 (function primitiveHashMultiply). */

/*	Implement 28-bit hashMultiply for SmallInteger and LargePositiveInteger
        receivers.
 */

/* InterpreterPrimitives>>#primitiveHashMultiply */

static void primitiveHashMultiply(void) {
  sqInt ccIndex;
  sqInt ok;
  char *sp;
  sqInt value;

  value = longAt(stackPointer);
  if ((((value) & 7) == 1)) {
    value = (value >> 3);
  } else {
    /* begin is:instanceOf:compactClassIndex: */
    if (((value & (tagMask())) != 0)) {
      ok = 0;
      goto l1;
    }

    /* begin isClassOfNonImm:equalTo:compactClassIndex: */
    assert(!(isImmediate(value)));
    ccIndex = (longAt((void *)(value))) & (classIndexMask());
    ok = ClassLargePositiveIntegerCompactIndex == ccIndex;
    /* end is:instanceOf:compactClassIndex: */
  l1:
    if (!ok) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadReceiver;
      return;
    }
    value = SQ_SWAP_4_BYTES_IF_BIGENDIAN(
        (long32At((void *)((value + BaseHeaderSize)))));
  }

  /* begin pop:thenPush: */
  longAtput(
      (sp = stackPointer),
      (((((((usqInt)value)) * HashMultiplyConstant) & 0xFFFFFFF) << 3) | 1));
  stackPointer = sp;
}