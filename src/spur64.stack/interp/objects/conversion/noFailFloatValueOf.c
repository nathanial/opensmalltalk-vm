/* Extracted from interp.c:27523 (function noFailFloatValueOf). */

/*	Answer the C double precision floating point value of the argument,
        which *must* be something for which self isFloatInstance: answers true.
        Note: May be called by translated primitive code. */

/* Spur64BitMemoryManager>>#noFailFloatValueOf: */

double noFailFloatValueOf(sqInt aFloatOop) {
  double result;

  assert(isFloatInstance(aFloatOop));
  if (aFloatOop & (tagMask())) {
    result = smallFloatValueOf(aFloatOop);
  } else {
    fetchFloatAtinto(aFloatOop + BaseHeaderSize, result);
  }
  return result;
}
