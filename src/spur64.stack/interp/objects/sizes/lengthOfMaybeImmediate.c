/* Extracted from interp.c:38124 (function lengthOfMaybeImmediate). */

/*	for the message send breakpoint; selectors can be immediates. */

/* SpurMemoryManager>>#lengthOfMaybeImmediate: */

static sqInt lengthOfMaybeImmediate(sqInt oop) {
  sqInt fmt;
  usqInt numSlots;
  if (((oop & (tagMask())) != 0)) {
    return 0;
  }

  /* begin lengthOf: */
  fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
  numSlots = numSlotsOfAny(oop);
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