/* Extracted from interp.c:27022 (function floatValueOf). */

/*	Answer the C double precision floating point value of the argument,
        or fail if it is not a Float, and answer 0.
        Note: May be called by translated primitive code. */

/* Spur64BitMemoryManager>>#floatValueOf: */

double floatValueOf(sqInt oop) {
  double result;
  sqInt tagBits;
  double value;

  if ((tagBits = oop & (tagMask()))) {
    if (tagBits == (smallFloatTag())) {
      value = smallFloatValueOf(oop);
      return value;
    }
  } else {
    if (((longAt((void *)(oop))) & (classIndexMask())) ==
        ClassFloatCompactIndex) {
      fetchFloatAtinto(oop + BaseHeaderSize, result);
      return result;
    }
  }

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
  return 0.0;
}
