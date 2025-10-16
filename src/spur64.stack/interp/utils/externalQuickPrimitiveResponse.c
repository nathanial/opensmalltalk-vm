/* Extracted from interp.c:52876 (function externalQuickPrimitiveResponse). */

/*	Invoke a quick primitive.
        Called under the assumption that primFunctionPtr has been preloaded */

/* StackInterpreter>>#externalQuickPrimitiveResponse */

static sqInt externalQuickPrimitiveResponse(void) {
  sqInt localPrimIndex;
  sqInt oop;

  assert(isPrimitiveFunctionPointerAnIndex());
  localPrimIndex = ((sqInt)primitiveFunctionPointer);
  assert((localPrimIndex > 0xFF) && (localPrimIndex < 520));

  /* Quick return inst vars */
  if (localPrimIndex >= 264) {
    oop = fetchPointerofObject(localPrimIndex - 264, longAt(stackPointer));

    popthenPush(1, oop);
    return 1;
  }

  /* Quick return constants */
  if (localPrimIndex == 0x100) {
    return 1;
  }
  if (localPrimIndex == 0x101) {
    popthenPush(1, trueObj);
    return 1;
  }
  if (localPrimIndex == 258) {
    popthenPush(1, falseObj);
    return 1;
  }
  if (localPrimIndex == 259) {
    popthenPush(1, nilObj);
    return 1;
  }

  popthenPush(1, (((usqInt)(localPrimIndex - 261) << 3) | 1));
  return 1;
}
