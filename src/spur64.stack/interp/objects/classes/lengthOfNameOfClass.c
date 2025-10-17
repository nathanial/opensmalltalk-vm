/* Extracted from interp.c:55518 (function lengthOfNameOfClass). */

/* StackInterpreter>>#lengthOfNameOfClass: */

static sqInt lengthOfNameOfClass(sqInt classOop) {
  sqInt fmt;
  usqInt numSlots;
  usqInt numSlots1;
  usqInt numSlotsUsqInt;
  sqInt objOop;

  
  numSlots =
      numSlotsOf(classOop);
  if (numSlots == metaclassNumSlots) {
    return lengthOfNameOfClass(fetchPointerofObject(thisClassIndex, classOop));
  }
  if (numSlots <= classNameIndex) {
    return 0;
  }
  objOop = fetchPointerofObject(classNameIndex, classOop);

  /* begin lengthOf: */
  fmt = (byteAt((void *)(objOop + (formatFieldByteOffset())))) & (formatMask());
  numSlotsUsqInt = byteAt((void *)(objOop + (numSlotsFieldByteOffset())));
  numSlots1 =
      (numSlotsUsqInt == (numSlotsMask())
           ? ((((usqInt)((
                 (sqInt)((usqInt)((longAt((void *)(objOop - BaseHeaderSize))))
                         << 8)))))) >>
                 8
           : numSlotsUsqInt);
  if (fmt <= (ephemeronFormat())) {
    return numSlots1;
  }
  if (fmt >= (firstByteFormat())) {
    return ((numSlots1 << (shiftForWord()))) - (fmt & 7);
  }

  /* bytes, including CompiledMethod */
  if (fmt >= (firstShortFormat())) {
    return ((numSlots1 << ((shiftForWord()) - 1))) - (fmt & 3);
  }
  if (fmt >= (firstLongFormat())) {
    return ((numSlots1 << ((shiftForWord()) - 2))) - (fmt & 1);
  }
  if (fmt == (sixtyFourBitIndexableFormat())) {
    return numSlots1;
  }

  /* fmt = self forwardedFormat */
  return 0;
}