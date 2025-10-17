/* Extracted from interp.c:17645 (function primitiveHighBit). */

/* InterpreterPrimitives>>#primitiveHighBit */

static void primitiveHighBit(void) {
  sqInt highestBitZeroBased;
  sqInt integerReceiverOop;
  sqInt leadingZeroCount;
  char *sp;

  integerReceiverOop = longAt(stackPointer);

  /* Convert the receiver Oop to use a single tag bit */
  integerReceiverOop =
      ((integerReceiverOop) >> 2 /* (numSmallIntegerTagBits - 1) */) | 1;
#if defined(__GNUC__)
  leadingZeroCount = __builtin_clzll(integerReceiverOop);
  if (!leadingZeroCount) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

  /* highBit is not defined for negative Integer
     Nice bit trick: 1-based high-bit is (32 - clz) - 1 to account for tag bit.
     This is like two-complement - clz - 1 on 5 bits, or in other words a
     bit-invert operation clz ^16r1F */

  /* begin pop:thenPushInteger: */
  longAtput((sp = stackPointer),
            (((usqInt)(leadingZeroCount ^ ((BytesPerWord * 8) - 1)) << 3) | 1));
  stackPointer = sp;
#elif (defined(_MSC_VER)) || (defined(__ICC))
  if (integerReceiverOop < 0) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }

  /* Setting this variable is useless, but VMMaker will generate an automatic
   * initialization at a worse place if this isn't initialized explicitly. */
  highestBitZeroBased = 0;

  /* We do not even test the return value, because integerReceiverOop is never
   * zero */
  _BitScanReverse64((&highestBitZeroBased), integerReceiverOop);

  /* thanks to the tag bit, the +1 operation for getting 1-based rank is not
   * necessary */

  /* begin pop:thenPushInteger: */
  longAtput((sp = stackPointer), (((usqInt)highestBitZeroBased << 3) | 1));
  stackPointer = sp;
#else // defined(__GNUC__) || (defined(_MSC_VER)) || (defined(__ICC))
  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
#endif
}