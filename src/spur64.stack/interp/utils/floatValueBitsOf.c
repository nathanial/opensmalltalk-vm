/* Extracted from interp.c:26995 (function floatValueBitsOf). */

/*	Answer the 64-bit value of the argument as raw bits. */

/* Spur64BitMemoryManager>>#floatValueBitsOf: */

static sqLong floatValueBitsOf(sqInt floatOop) {
  usqLong rot;

  assert(isFloatInstance(floatOop));
  if (floatOop & (tagMask())) {
    /* begin smallFloatBitsOf: */
    assert(isImmediateFloat(floatOop));
    rot = ((((usqInt)floatOop))) >> (numTagBits());
    if (rot > 1) {
      rot += (((usqInt)((smallFloatExponentOffset()))
               << ((smallFloatMantissaBits()) + 1)));
    }

    /* a.k.a. ~= +/-0.0 */
    rot = ((rot << 0x3F)) + (((((usqInt)rot))) >> 1);
    return rot;
  }
  return long64At((void *)((floatOop + BaseHeaderSize)));
}