/* Extracted from interp.c:36260 (function indexOfin). */

/* IMMUTABILITY */

/* SpurMemoryManager>>#indexOf:in: */

sqInt indexOfin(sqInt anElement, sqInt anObject) {
  sqInt fmt;
  sqInt fmtSqInt;
  sqInt i;
  usqInt numSlots;
  fmt =
      (byteAt((void *)(anObject + (formatFieldByteOffset())))) & (formatMask());
  if (fmt <= 5 /* lastPointerFormat */) {

    numSlots = numSlotsOf(anObject);
    for (i = 0; i <= numSlots; i += 1) {
      if (anElement == (fetchPointerofObject(i, anObject))) {
        return i;
      }
    }
  }
  if (fmt >= (firstByteFormat())) {
    if (fmt >= (firstCompiledMethodFormat())) {
      return (primFailCode = PrimErrUnsupported);
    }

    /* begin numBytesOfBytes: */
    fmtSqInt = (byteAt((void *)(anObject + (formatFieldByteOffset())))) &
               (formatMask());
    assert(fmtSqInt >= (firstByteFormat()));
    numSlots = ((((assert((classIndexOf(anObject)) >
                          (isForwardedObjectClassIndexPun())),
                   numSlotsOf(anObject)))
                 << (shiftForWord()))) -
               (fmtSqInt & 7);
    for (i = 0; i <= numSlots; i += 1) {
      if (anElement == (byteAt((void *)((anObject + BaseHeaderSize) + i)))) {
        return i;
      }
    }
  }
  if (fmt >= (firstShortFormat())) {
    numSlots = ((usqInt)((numBytesOf(anObject)))) >> 1;
    for (i = 0; i <= numSlots; i += 1) {
      if (anElement ==
          (((unsigned short)(shortAt((void *)((anObject + BaseHeaderSize) +
                                              ((((usqInt)(i) << 1))))))))) {
        return i;
      }
    }
  }
  if (fmt == (sixtyFourBitIndexableFormat())) {
    numSlots = ((usqInt)((numBytesOf(anObject)))) >> 3;
    for (i = 0; i <= numSlots; i += 1) {
      if (anElement == (long64At((void *)((anObject + BaseHeaderSize) +
                                          ((((usqInt)(i) << 3))))))) {
        return i;
      }
    }
  }
  if (fmt >= (firstLongFormat())) {
    numSlots = ((usqInt)((numBytesOf(anObject)))) >> 2;
    for (i = 0; i <= numSlots; i += 1) {
      if (anElement == (long32At((void *)((anObject + BaseHeaderSize) +
                                          ((((usqInt)(i) << 2))))))) {
        return i;
      }
    }
  }
  return -1;
}