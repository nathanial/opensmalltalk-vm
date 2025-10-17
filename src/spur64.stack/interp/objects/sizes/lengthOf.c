/* Extracted from interp.c:38169 (function lengthOf). */

/*	Answer the number of indexable units in the given object.
        For a CompiledMethod, the size of the method header (in bytes) should
        be subtracted from the result. */

/* SpurMemoryManager>>#lengthOf: */

static sqInt lengthOf(sqInt objOop) {
  sqInt fmt;
  usqInt numSlots;
  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

  /* begin lengthOf:format: */
  numSlots = numSlotsOfAny(objOop);
  if (fmt <= (ephemeronFormat())) {
    return numSlots;
  }
  if (fmt >= (firstByteFormat())) {
    return ((numSlots << (shiftForWord()))) - (fmt & 7);
  }

  /* bytes, including CompiledMethod */
  if (fmt >= (firstShortFormat())) {
    return ((numSlots << ((shiftForWord()) - 1))) - (fmt & 3);
  }
  if (fmt >= (firstLongFormat())) {
    return ((numSlots << ((shiftForWord()) - 2))) - (fmt & 1);
  }
  if (fmt == (sixtyFourBitIndexableFormat())) {
    return numSlots;
  }

  /* fmt = self forwardedFormat */
  return 0;
}