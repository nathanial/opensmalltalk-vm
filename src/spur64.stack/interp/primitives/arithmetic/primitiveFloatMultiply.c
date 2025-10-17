/* Extracted from interp.c:16138 (function primitiveFloatMultiply). */

/* InterpreterPrimitives>>#primitiveFloatMultiply */

static void primitiveFloatMultiply(void) {
  double arg;
  sqInt argOop;
  usqLong bits;
  double rcvr;
  sqInt rcvrOop;
  double result;
  char *sp;
  sqInt tagBits;
  double value;

  rcvrOop = longAt(stackPointer + (1 * BytesPerWord));
  argOop = longAt(stackPointer);

  /* begin primitiveFloatMultiply:byArg: */
  /* begin loadFloatOrIntFrom: */
  if ((tagBits = rcvrOop & (tagMask()))) {
    if (tagBits == (smallFloatTag())) {
      value = smallFloatValueOf(rcvrOop);
      rcvr = value;
      goto l1;
    }
    if ((primitiveDoMixedArithmetic) && (tagBits == (smallIntegerTag()))) {
      rcvr = ((double)((rcvrOop >> 3)));
      goto l1;
    }
  } else {
    if (((longAt((void *)(rcvrOop))) & (classIndexMask())) ==
        ClassFloatCompactIndex) {
      fetchFloatAtinto(rcvrOop + BaseHeaderSize, result);
      rcvr = result;
      goto l1;
    }
  }

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
  rcvr = 0.0;
  /* end loadFloatOrIntFrom: */
l1:

  /* begin loadFloatOrIntFrom: */
  if ((tagBits = argOop & (tagMask()))) {
    if (tagBits == (smallFloatTag())) {
      value = smallFloatValueOf(argOop);
      arg = value;
      goto l2;
    }
    if ((primitiveDoMixedArithmetic) && (tagBits == (smallIntegerTag()))) {
      arg = ((double)((argOop >> 3)));
      goto l2;
    }
  } else {
    if (((longAt((void *)(argOop))) & (classIndexMask())) ==
        ClassFloatCompactIndex) {
      fetchFloatAtinto(argOop + BaseHeaderSize, result);
      arg = result;
      goto l2;
    }
  }

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
  arg = 0.0;
  /* end loadFloatOrIntFrom: */
l2:
  if (!primFailCode) {
    /* begin pop:thenPushFloat: */
    longAtput((sp = stackPointer + (1 * BytesPerWord)),
              floatObjectOf(rcvr * arg));
    stackPointer = sp;
  }
}