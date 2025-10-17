/* Extracted from interp.c:13069 (function primitiveBitXor). */

/* InterpreterPrimitives>>#primitiveBitXor */

static void primitiveBitXor(void) {
  sqInt integerArgument;
  usqInt integerArgumentValue;
  sqInt integerReceiver;
  usqInt integerReceiverValue;
  sqInt oop;
  char *sp;
  sqInt value;

  integerArgument = longAt(stackPointer);
  integerReceiver = longAt(stackPointer + (1 * BytesPerWord));
  if (((((integerArgument) & 7) == 1)) && ((((integerReceiver) & 7) == 1))) {
    oop = (integerArgument ^ integerReceiver) + (smallIntegerTag());

    /* begin pop:thenPush: */
    longAtput((sp = stackPointer + (1 * BytesPerWord)), oop);
    stackPointer = sp;
  } else {
    /* begin positiveMachineIntegerValueOf: */
    if ((((integerArgument) & 7) == 1)) {
      value = (integerArgument >> 3);
      if (value < 0) {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        integerArgumentValue = ((usqIntptr_t)null);
        goto l1;
      }
      integerArgumentValue = ((usqIntptr_t)value);
      goto l1;
    }

    /* don't inline the rare case */
    integerArgumentValue = positiveMachineIntegerValueOfObj(integerArgument);
    /* end positiveMachineIntegerValueOf: */
  l1:

    /* begin positiveMachineIntegerValueOf: */
    if ((((integerReceiver) & 7) == 1)) {
      value = (integerReceiver >> 3);
      if (value < 0) {
        /* begin primitiveFail */
        if (!primFailCode) {
          primFailCode = 1;
        }
        integerReceiverValue = ((usqIntptr_t)null);
        goto l2;
      }
      integerReceiverValue = ((usqIntptr_t)value);
      goto l2;
    }

    /* don't inline the rare case */
    integerReceiverValue = positiveMachineIntegerValueOfObj(integerReceiver);
    /* end positiveMachineIntegerValueOf: */
  l2:
    if (!primFailCode) {
      oop =
          positive64BitIntegerFor(integerArgumentValue ^ integerReceiverValue);

      /* begin pop:thenPush: */
      longAtput((sp = stackPointer + (1 * BytesPerWord)), oop);
      stackPointer = sp;
    }
  }
}