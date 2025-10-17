/* Extracted from interp.c:51435 (function classNameOfIs). */

/*	Check if aClass's name is className */

/* StackInterpreter>>#classNameOf:Is: */

static sqInt classNameOfIs(sqInt aClass, char *className) {
  sqInt fmt;
  sqInt i;
  sqInt length;
  sqInt name;
  usqInt numSlots;
  char *srcName;

  if (((
        numSlotsOf(aClass))) <= classNameIndex) {
    return 0;
  }
  name = fetchPointerofObject(classNameIndex, aClass);
  if (!(/* isBytes: */
        ((!(name & (tagMask())))) &&
        (((byteAt((void *)(name + (formatFieldByteOffset())))) &
          (formatMask())) >= (firstByteFormat())))) {
    return 0;
  }

  /* begin numBytesOfBytes: */
  fmt = (byteAt((void *)(name + (formatFieldByteOffset())))) & (formatMask());
  assert(fmt >= (firstByteFormat()));
  length =
      ((((
          numSlotsOf(name)))
        << (shiftForWord()))) -
      (fmt & 7);
  srcName = ((char *)(arrayValueOf(name)));
  for (i = 0; i < length; i += 1) {
    if (!((srcName[i]) == (className[i]))) {
      return 0;
    }
  }

  /* Check if className really ends at this point */
  return (className[length]) == 0;
}