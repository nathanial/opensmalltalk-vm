/* Extracted from interp.c:27463 (function loadFloatOrIntFrom). */

/*	If floatOrInt is an integer and we enable mixed arithmetic in
   primitives, then convert it to a C double float and return it. If it is a
   Float, then load its value and return it. Otherwise fail -- ie return with
   primErrorCode non-zero. */

/* Spur64BitMemoryManager>>#loadFloatOrIntFrom: */

static double loadFloatOrIntFrom(sqInt floatOrIntOop) {
  usqLong bits;
  double result;
  sqInt tagBits;
  double value;

  if ((tagBits = floatOrIntOop & (tagMask()))) {
    if (tagBits == (smallFloatTag())) {
      value = smallFloatValueOf(floatOrIntOop);
      return value;
    }
    if ((primitiveDoMixedArithmetic) && (tagBits == (smallIntegerTag()))) {
      return ((double)((floatOrIntOop >> 3)));
    }
  } else {
    if (((longAt((void *)(floatOrIntOop))) & (classIndexMask())) ==
        ClassFloatCompactIndex) {
      fetchFloatAtinto(floatOrIntOop + BaseHeaderSize, result);
      return result;
    }
  }

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
  return 0.0;
}