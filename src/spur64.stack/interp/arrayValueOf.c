/* Extracted from interp.c:50145 (function arrayValueOf). */

/*	Return the address of first indexable field of resulting array object,
   or fail if the instance variable does not contain an indexable bytes or words
   object. */
/*	Note: May be called by translated primitive code. */

/* StackInterpreter>>#arrayValueOf: */

void *arrayValueOf(sqInt arrayOop) {
  if (/* isWordsOrBytes: */
      ((!(arrayOop & (tagMask())))) && (isWordsOrBytesNonImm(arrayOop))) {
    return ((void *)(pointerForOop(arrayOop + BaseHeaderSize)));
  }

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
  return 0;
}