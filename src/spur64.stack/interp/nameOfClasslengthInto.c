/* Extracted from interp.c:58085 (function nameOfClasslengthInto). */

/*	Brain-damaged nameOfClass: for C VM. Does *not* answer Foo class for
        metaclasses. Use e.g. classIsMeta: to avoid being fooled. */

/* StackInterpreter>>#nameOfClass:lengthInto: */

static char *nameOfClasslengthInto(sqInt classOop,
                                                 sqInt *lengthPtr) {
  sqInt maybeNameOop;
  sqInt maybeThisClassOop;
  usqInt numSlots;
  usqInt numSlotsUsqInt;

  /* begin numSlotsOf: */
  assert((classIndexOf(classOop)) > (isForwardedObjectClassIndexPun()));
  numSlots =
      (((numSlotsUsqInt =
             byteAt((void *)(classOop + (numSlotsFieldByteOffset()))))) ==
               (numSlotsMask())
           ? ((((usqInt)((
                 (sqInt)((usqInt)((longAt((void *)(classOop - BaseHeaderSize))))
                         << 8)))))) >>
                 8
           : numSlotsUsqInt);
  if (numSlots == metaclassNumSlots) {
    maybeThisClassOop =
        longAt((void *)((classOop + BaseHeaderSize) +
                        ((((usqInt)(thisClassIndex) << (shiftForWord()))))));
    if (addressCouldBeClassObj(maybeThisClassOop)) {
      return nameOfClasslengthInto(
          longAt((void *)((classOop + BaseHeaderSize) +
                          ((((usqInt)(thisClassIndex) << (shiftForWord())))))),
          lengthPtr);
    }
    lengthPtr[0] = 9;
    return "bad class";
  }
  if ((numSlots > classNameIndex) &&
      (((maybeNameOop = longAt(
             (void *)((classOop + BaseHeaderSize) +
                      ((((usqInt)(classNameIndex) << (shiftForWord()))))))),
        /* isBytes: */
        ((!(maybeNameOop & (tagMask())))) &&
            (((byteAt((void *)(maybeNameOop + (formatFieldByteOffset())))) &
              (formatMask())) >= (firstByteFormat()))))) {
    lengthPtr[0] = (numBytesOfBytes(maybeNameOop));
    return firstIndexableField(maybeNameOop);
  }
  lengthPtr[0] = 9;
  return "bad class";
}