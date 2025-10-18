/* Extracted from interp.c:15939 (function primitiveFloatDivide). */

/* InterpreterPrimitives>>#primitiveFloatDivide */

static void primitiveFloatDivide(void) {
  double arg;
  sqInt argOop;
  usqLong bits;
  double rcvr;
  sqInt rcvrOop;
  double result;
  sqInt tagBits;
  double value;

  rcvrOop = longAt(stackPointer + (1 * BytesPerWord));
  argOop = longAt(stackPointer);

  /* begin primitiveFloatDivide:byArg: */
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

  /* begin success: */
  if (!(arg != 0.0)) {
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
  if (!primFailCode) {
    /* begin pop:thenPushFloat: */
    popthenPushFloat(2, floatObjectOf(rcvr / arg));
  }
}
