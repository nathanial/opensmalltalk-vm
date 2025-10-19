/* Extracted from interp.c:58617 (function primitiveFloatGreaterOrEqualtoArg).
 */

/* StackInterpreter>>#primitiveFloatGreaterOrEqual:toArg: */

static int primitiveFloatGreaterOrEqualtoArg(sqInt rcvrOop, sqInt argOop) {
  double arg;
  double rcvr;
  double result;
  sqInt tagBits;
  double value;

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
  if (rcvr == arg) {
    if ((((argOop) & 7) == 1)) {
      return (((sqInt)rcvr)) >= ((argOop >> 3));
    }

    /* Resolve case of ambiguity so as to have comparison of exact values */
    if ((((rcvrOop) & 7) == 1)) {
      return ((rcvrOop >> 3)) >= (((sqInt)arg));
    }
  }
  return rcvr >= arg;
}