/* Extracted from interp.c:58085 (function nameOfClasslengthInto). */

/*	Brain-damaged nameOfClass: for C VM. Does *not* answer Foo class for
        metaclasses. Use e.g. classIsMeta: to avoid being fooled. */

/* StackInterpreter>>#nameOfClass:lengthInto: */

static char *nameOfClasslengthInto(sqInt classOop, sqInt *lengthPtr) {
  sqInt maybeNameOop;
  sqInt maybeThisClassOop;
  usqInt numSlots;
  numSlots = numSlotsOf(classOop);
  if (numSlots == metaclassNumSlots) {
    maybeThisClassOop = fetchPointerofObject(thisClassIndex, classOop);
    if (addressCouldBeClassObj(maybeThisClassOop)) {
      return nameOfClasslengthInto(
          fetchPointerofObject(thisClassIndex, classOop), lengthPtr);
    }
    lengthPtr[0] = 9;
    return "bad class";
  }
  if ((numSlots > classNameIndex) &&
      (((maybeNameOop = fetchPointerofObject(classNameIndex, classOop)),
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