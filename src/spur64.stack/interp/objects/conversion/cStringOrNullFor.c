/* Extracted from interp.c:11049 (function cStringOrNullFor). */

/*	Answer either a malloced string with the null-terminated contents of oop
        if oop is a string,
        or the null pointer if oop is nil, or fail. It is the client's
        responsibility to free the string later. */

/* InterpreterPrimitives>>#cStringOrNullFor: */

char *cStringOrNullFor(sqInt oop) {
  sqInt ccIndex;
  char *cString;
  sqInt isString;
  sqInt len;
  /* begin isInstanceOfClassByteString: */
  /* begin is:instanceOf:compactClassIndex: */
  if (((oop & (tagMask())) != 0)) {
    isString = 0;
    goto l2;
  }

  /* begin isClassOfNonImm:equalTo:compactClassIndex: */
  assert(!(isImmediate(oop)));
  ccIndex = (longAt((void *)(oop))) & (classIndexMask());
  isString = ClassByteStringCompactIndex == ccIndex;
  /* end isInstanceOfClassByteString: */
l2:
  if (!isString) {
    if (oop != nilObj) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
    }
    return 0;
  }

  len = lengthOf(oop);
  if (!len) {
    return 0;
  }
  cString = malloc(len + 1);
  if (!cString) {
    /* primitiveFailFor: */
    primFailCode = PrimErrNoCMemory;
    return 0;
  }
  memcpy(cString, firstIndexableField(oop), len);
  cString[len] = 0;
  return cString;
}
