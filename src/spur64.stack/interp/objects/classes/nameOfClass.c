/* Extracted from interp.c:58050 (function nameOfClass). */

/*	Brain-damaged nameOfClass: for C VM. Does *not* answer Foo class for
        metaclasses. Use e.g. classIsMeta: to avoid being fooled. */

/* StackInterpreter>>#nameOfClass: */

static char *nameOfClass(sqInt classOop) {
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
        fetchPointerofObject(thisClassIndex, classOop);
    if (addressCouldBeClassObj(maybeThisClassOop)) {
      return nameOfClass(
          fetchPointerofObject(thisClassIndex, classOop));
    }
    return "bad class";
  }
  if (!((numSlots > classNameIndex) &&
        (((maybeNameOop = fetchPointerofObject(classNameIndex, classOop)),
          /* isBytes: */
          ((!(maybeNameOop & (tagMask())))) &&
              (((byteAt((void *)(maybeNameOop + (formatFieldByteOffset())))) &
                (formatMask())) >= (firstByteFormat())))))) {
    return "bad class";
  }
  return firstIndexableField(maybeNameOop);
}