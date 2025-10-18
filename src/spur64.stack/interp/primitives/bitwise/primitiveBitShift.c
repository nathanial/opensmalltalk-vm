/* Extracted from interp.c:12937 (function primitiveBitShift). */

/*	Perform a bitShift. In 32-bits deal only with non-negative 32-bit
        integers. In 64-bits deal with signed 64-bit quantities (max (2^63)-1).
 */

/* InterpreterPrimitives>>#primitiveBitShift */

static void primitiveBitShift(void) {
  sqInt integerArgument;
  sqInt integerReceiver;
  sqInt shifted;
  char *sp;

  integerArgument = longAt(stackPointer);
  if (!((((integerArgument) & 7) == 1))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return;
  }
  integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
  integerReceiver = signed64BitValueOf(integerReceiver);
  if (!primFailCode) {
    if (((integerArgument = (integerArgument >> 3))) >= 0) {
      if (!(integerArgument <= 61 /* numSmallIntegerBits */)) {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        return;
      }
      shifted = ((sqInt)((usqInt)(integerReceiver) << integerArgument));
      if (!(integerReceiver == ((shifted) >> integerArgument))) {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        return;
      }
    } else {
      if (!(integerArgument >= (-61 /* numSmallIntegerBits */))) {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        return;
      }
      shifted = (integerReceiver) >> (0 - integerArgument);
    }

    /* Left shift -- must fail bits would be lost
       Right shift -- OK to lose bits */
    shifted = ((((((usqInt)(shifted)) >> 60) + 1) & 15) <= 1
                   ? (((usqInt)shifted << 3) | 1)
                   : signed64BitIntegerFor(shifted));

    /* begin pop:thenPush: */
    popthenPush(2, shifted);
  }
}