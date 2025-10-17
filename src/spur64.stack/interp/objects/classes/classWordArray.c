/* Extracted from interp.c:33425 (function classWordArray). */

/* SpurMemoryManager>>#classWordArray */

sqInt classWordArray(void) {
  sqInt classOop;
  sqInt hash;

  /* begin accessIntegerClass:withValidationFlag: */
  hash = 0;
  classOop =
      fetchPointerofObject(ClassWordArray, specialObjectsOop);
  if (((validatedIntegerClassFlags & ValidatedClassWordArrayFlag) != 0)) {
    return classOop;
  }
  if ((((classOop & (tagMask())) != 0)) ||
      ((((hash = (long32At((void *)(classOop + 4))) &
                 (identityHashHalfWordMask()))) == 0) ||
       ((classAtIndex(hash)) != classOop))) {
    return nilObj;
  }
  validatedIntegerClassFlags =
      validatedIntegerClassFlags | ValidatedClassWordArrayFlag;
  return classOop;
}