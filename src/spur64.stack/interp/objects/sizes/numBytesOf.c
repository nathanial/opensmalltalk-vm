/* Extracted from interp.c:41095 (function numBytesOf). */

/*	Answer the number of indexable bytes in the given non-immediate object.
        Does not adjust the size of contexts by stackPointer. */

/* SpurMemoryManager>>#numBytesOf: */

sqInt numBytesOf(sqInt objOop) {
  sqInt fmt;
  usqInt numBytes;
  usqInt numSlots;

  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());

  numBytes = numSlotsOf(objOop);
  numBytes = (numBytes << (shiftForWord()));
  if (fmt >= (firstByteFormat())) {
    return numBytes - (fmt & 7);
  }

  /* bytes (the common case), including CompiledMethod */
  if (fmt <= (sixtyFourBitIndexableFormat())) {
    return numBytes;
  }
  if (fmt >= (firstShortFormat())) {
    return numBytes - (((fmt & 3) << 1));
  }

  /* fmt >= self firstLongFormat */
  return numBytes - (((fmt & 1) << 2));
}