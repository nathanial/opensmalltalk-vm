/* Extracted from interp.c:41044 (function numBytesOfBitsformat). */

/*	Answer the number of indexable bytes in the given non-immediate pure
   bits object with format.
 */

/* SpurMemoryManager>>#numBytesOfBits:format: */

static sqInt numBytesOfBitsformat(sqInt objOop, sqInt format) {
  usqInt numBytes;
  usqInt numSlots;

  assert((isPureBitsFormat(format)) && ((formatOf(objOop)) == format));

  
  numBytes =
      numSlotsOf(objOop);
  numBytes = (numBytes << (shiftForWord()));
  if (format < (firstShortFormat())) {
    return (format <= (sixtyFourBitIndexableFormat())
                ? numBytes
                : numBytes - (((format & 1) << 2)));
  }
  return (format < (firstByteFormat()) ? numBytes - (((format & 3) << 1))
                                       : numBytes - (format & 7));
}