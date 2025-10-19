/* Extracted from interp.c:23079 (function primitiveSmallFloatEqual). */

/* InterpreterPrimitives>>#primitiveSmallFloatEqual */

static void primitiveSmallFloatEqual(void) {
  double arg;
  sqInt floatOrIntOop;
  sqInt intArg;
  sqInt oop;
  double rcvr;
  double result;
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
    if ((rcvr == arg) && (((((longAt(stackPointer))) & 7) == 1))) {
      intArg = ((longAt(stackPointer)) >> 3);

      /* begin pop:thenPushBool: */
      popthenPushBool(2, booleanObjectOf((((sqInt)rcvr)) == intArg));
    } else {
      /* begin pop:thenPushBool: */
      popthenPushBool(2, booleanObjectOf(rcvr == arg));
    }
  }
}