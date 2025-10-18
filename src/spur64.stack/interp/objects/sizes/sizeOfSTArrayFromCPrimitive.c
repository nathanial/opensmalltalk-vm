/* Extracted from interp.c:63516 (function sizeOfSTArrayFromCPrimitive). */

/*	Return the number of indexable fields of the given object. This method
   is to be called from an automatically generated C primitive. The argument is
        assumed to be a pointer to the first indexable field of a words or bytes
        object; the object header starts 4 bytes before that.
 */
/*	Note: Only called by translated primitive code. */

/* StackInterpreter>>#sizeOfSTArrayFromCPrimitive: */

sqInt sizeOfSTArrayFromCPrimitive(void *cPtr) {
  sqInt oop;

  oop = (oopForPointer(cPtr)) - BaseHeaderSize;
  if (!(/* isWordsOrBytes: */
        ((!(oop & (tagMask())))) && (isWordsOrBytesNonImm(oop)))) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return 0;
  }

  return lengthOf(oop);
}
