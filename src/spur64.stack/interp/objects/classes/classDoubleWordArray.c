/* Extracted from interp.c:33106 (function classDoubleWordArray). */

/* SpurMemoryManager>>#classDoubleWordArray */

sqInt classDoubleWordArray(void) {
  sqInt classOop;
  sqInt hash;

  /* begin accessIntegerClass:withValidationFlag: */
  hash = 0;
  classOop = fetchPointerofObject(ClassDoubleWordArray, specialObjectsOop);
  if (((validatedIntegerClassFlags & ValidatedClassDoubleWordArrayFlag) != 0)) {
    return classOop;
  }
  if ((((classOop & (tagMask())) != 0)) ||
      ((((hash = (long32At((void *)(classOop + 4))) &
                 (identityHashHalfWordMask()))) == 0) ||
       ((classAtIndex(hash)) != classOop))) {
    return nilObj;
  }
  validatedIntegerClassFlags =
      validatedIntegerClassFlags | ValidatedClassDoubleWordArrayFlag;
  return classOop;
}