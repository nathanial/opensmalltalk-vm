/* Extracted from interp.c:31796 (function byteSizeOf). */

/* SpurMemoryManager>>#byteSizeOf: */

sqInt byteSizeOf(sqInt oop) {
  sqInt fmt;
  usqInt numBytes;

  if (((oop & (tagMask())) != 0)) {
    return 0;
  }

  /* begin numBytesOf: */
  fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
  numBytes = numSlotsOf(oop);
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
