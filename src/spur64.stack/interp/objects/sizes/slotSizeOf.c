/* Extracted from interp.c:45638 (function slotSizeOf). */

/*	*DO NOT CONFUSE THIS WITH numSlotsOf:.
        This is an ObjectMemory compatibility method with questionable
   semantics. Answers the number of slots in the receiver. If the receiver is a
   byte object, return the number of bytes. If the receiver is a short object,
   return the number of shorts. If the receiver is a word object, return the
   number of words. If the receiver is a double word object, return the number
   of double words. Otherwise return the number of pointers. */

/* SpurMemoryManager>>#slotSizeOf: */

sqInt slotSizeOf(sqInt oop) {
  sqInt fmt;
  usqInt numSlots;
  usqInt numSlotsUsqInt;

  if (((oop & (tagMask())) != 0)) {
    return 0;
  }

  /* begin lengthOf: */
  fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
  numSlotsUsqInt = byteAt((void *)(oop + (numSlotsFieldByteOffset())));
  numSlots =
      (numSlotsUsqInt == (numSlotsMask())
           ? ((((usqInt)((
                 (sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize))))
                         << 8)))))) >>
                 8
           : numSlotsUsqInt);
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