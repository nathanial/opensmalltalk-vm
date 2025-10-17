/* Extracted from interp.c:23862 (function primitiveSmallFloatSubtract). */

/* InterpreterPrimitives>>#primitiveSmallFloatSubtract */

static void primitiveSmallFloatSubtract(void) {
  double arg;
  usqLong bits;
  sqInt floatOrIntOop;
  sqInt oop;
  double rcvr;
  double result;
  char *sp;
  sqInt tagBits;
  double value;

  oop = longAt(stackPointer + (1 * BytesPerWord));

  rcvr = smallFloatValueOf(oop);
  floatOrIntOop = longAt(stackPointer);

  /* begin loadFloatOrIntFrom: */
  if ((tagBits = floatOrIntOop & (tagMask()))) {
    if (tagBits == (smallFloatTag())) {
      value = smallFloatValueOf(floatOrIntOop);
      arg = value;
      goto l1;
    }
    if ((primitiveDoMixedArithmetic) && (tagBits == (smallIntegerTag()))) {
      arg = ((double)((floatOrIntOop >> 3)));
      goto l1;
    }
  } else {
    if (((longAt((void *)(floatOrIntOop))) & (classIndexMask())) ==
        ClassFloatCompactIndex) {
      fetchFloatAtinto(floatOrIntOop + BaseHeaderSize, result);
      arg = result;
      goto l1;
    }
  }

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
  arg = 0.0;
  /* end loadFloatOrIntFrom: */
l1:
  if (!primFailCode) {
    /* begin pop:thenPushFloat: */
    longAtput((sp = stackPointer + (1 * BytesPerWord)),
              floatObjectOf(rcvr - arg));
    stackPointer = sp;
  }
}