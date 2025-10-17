/* Extracted from interp.c:33084 (function classDoubleByteArray). */

/* SpurMemoryManager>>#classDoubleByteArray */

sqInt classDoubleByteArray(void) {
  sqInt classOop;
  sqInt hash;

  /* begin accessIntegerClass:withValidationFlag: */
  hash = 0;
  classOop = fetchPointerofObject(ClassDoubleByteArray, specialObjectsOop);
  if (((validatedIntegerClassFlags & ValidatedClassDoubleByteArrayFlag) != 0)) {
    return classOop;
  }
  if ((((classOop & (tagMask())) != 0)) ||
      ((((hash = (long32At((void *)(classOop + 4))) &
                 (identityHashHalfWordMask()))) == 0) ||
       ((classAtIndex(hash)) != classOop))) {
    return nilObj;
  }
  validatedIntegerClassFlags =
      validatedIntegerClassFlags | ValidatedClassDoubleByteArrayFlag;
  return classOop;
}