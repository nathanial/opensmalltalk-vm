/* Extracted from interp.c:52935 (function fetchArrayofObject). */

/*	Fetch the instance variable at the given index of the given object.
   Return the address of first indexable field of resulting array object, or
   fail if the instance variable does not contain an indexable bytes or words
   object.
 */
/*	Note: May be called by translated primitive code. */

/* StackInterpreter>>#fetchArray:ofObject: */

void *fetchArrayofObject(sqInt fieldIndex, sqInt objectPointer) {
  sqInt arrayOop;

  arrayOop = longAt((void *)((objectPointer + BaseHeaderSize) +
                             ((((usqInt)(fieldIndex) << (shiftForWord()))))));

  /* begin arrayValueOf: */
  if (/* isWordsOrBytes: */
      ((!(arrayOop & (tagMask())))) && (isWordsOrBytesNonImm(arrayOop))) {
    return ((void *)(pointerForOop(arrayOop + BaseHeaderSize)));
  }

  /* begin primitiveFail */
  if (!primFailCode) {
    primFailCode = 1;
  }
  return null;
}