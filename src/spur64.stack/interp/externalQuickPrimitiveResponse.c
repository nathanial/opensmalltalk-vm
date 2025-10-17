/* Extracted from interp.c:52876 (function externalQuickPrimitiveResponse). */

/*	Invoke a quick primitive.
        Called under the assumption that primFunctionPtr has been preloaded */

/* StackInterpreter>>#externalQuickPrimitiveResponse */

static sqInt externalQuickPrimitiveResponse(void) {
  sqInt localPrimIndex;
  sqInt oop;
  char *sp;

  assert(isPrimitiveFunctionPointerAnIndex());
  localPrimIndex = ((sqInt)primitiveFunctionPointer);
  assert((localPrimIndex > 0xFF) && (localPrimIndex < 520));

  /* Quick return inst vars */
  if (localPrimIndex >= 264) {
    oop = longAt(
        (void *)(((longAt(stackPointer)) + BaseHeaderSize) +
                 ((((usqInt)((localPrimIndex - 264)) << (shiftForWord()))))));

    /* begin pop:thenPush: */
    longAtput((sp = stackPointer), oop);
    stackPointer = sp;
    return 1;
  }

  /* Quick return constants */
  if (localPrimIndex == 0x100) {
    return 1;
  }
  if (localPrimIndex == 0x101) {
    /* begin pop:thenPush: */
    longAtput((sp = stackPointer), trueObj);
    stackPointer = sp;
    return 1;
  }
  if (localPrimIndex == 258) {
    /* begin pop:thenPush: */
    longAtput((sp = stackPointer), falseObj);
    stackPointer = sp;
    return 1;
  }
  if (localPrimIndex == 259) {
    /* begin pop:thenPush: */
    longAtput((sp = stackPointer), nilObj);
    stackPointer = sp;
    return 1;
  }

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer), (((usqInt)(localPrimIndex - 261) << 3) | 1));
  stackPointer = sp;
  return 1;
}