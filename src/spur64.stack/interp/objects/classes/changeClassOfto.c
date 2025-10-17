/* Extracted from interp.c:31874 (function changeClassOfto). */

/*	Attempt to change the class of the receiver to the argument given that
   the format of the receiver matches the format of the argument. If successful,
        answer 0, otherwise answer an error code indicating the reason for
        failure.
        Fail if the format of the receiver is incompatible with the format of
   the argument, or if the argument is a fixed class and the receiver's size
        differs from the size
        that an instance of the argument should have. */

/* SpurMemoryManager>>#changeClassOf:to: */

static sqInt changeClassOfto(sqInt rcvr, sqInt argClass) {
  sqInt classFormat;
  sqInt classIndex;
  sqInt fixedFields;
  sqInt fmt;
  sqInt instBytes;
  sqInt instFormat;
  usqInt instSlots;
  sqInt newFormat;
  usqInt numBytes;
  usqInt numSlots;

  instBytes = 0;
  instSlots = 0;
  if (
#if IMMUTABILITY
      ((((usqInt)((
            byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >>
        (immutableBitByteShift())) &
       1) != 0
#else
      0
#endif
  ) {
    return PrimErrNoModification;
  }
  classFormat =
      ((fetchPointerofObject(InstanceSpecificationIndex, argClass)) >> 3);
  fixedFields = classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
  classFormat =
      (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
  instFormat =
      (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());

  /* Fail for inability to access classIndex before making contexts
   * snapshot-safe. */
  if (((classIndex = ensureBehaviorHash(argClass))) < 0) {
    return -classIndex;
  }

  /* Now check the instance for compatibility and compute odd bits if necessary.
   */
  if (classFormat <= 5 /* lastPointerFormat */) {
    if (instFormat > 5 /* lastPointerFormat */) {
      return PrimErrInappropriate;
    }
    if ((((
           (((instSlots = numSlotsOf(rcvr)))))) < fixedFields) ||
        ((instSlots > fixedFields) &&
         (/* isFixedSizePointerFormat: */
          (classFormat <= (nonIndexablePointerFormat())) ||
          (classFormat == (ephemeronFormat()))))) {
      return PrimErrBadReceiver;
    }
    if ((instFormat == (indexablePointersFormat())) &&
        (((longAt((void *)(rcvr))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex)) {
      makeContextSnapshotSafe(rcvr);
    }
    newFormat = classFormat;
  } else {
    if (classFormat != (classFormatFromInstFormat(classFormat))) {
      return PrimErrBadArgument;
    }

    /* begin numBytesOf: */
    fmt = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask());
    numBytes =
        numSlotsOf(rcvr);
    numBytes = (numBytes << (shiftForWord()));
    if (fmt >= (firstByteFormat())) {
      instBytes = numBytes - (fmt & 7);
      goto l1;
    }

    /* bytes (the common case), including CompiledMethod */
    if (fmt <= (sixtyFourBitIndexableFormat())) {
      instBytes = numBytes;
      goto l1;
    }
    if (fmt >= (firstShortFormat())) {
      instBytes = numBytes - (((fmt & 3) << 1));
      goto l1;
    }

    /* fmt >= self firstLongFormat */
    instBytes = numBytes - (((fmt & 1) << 2));
    /* end numBytesOf: */
  l1:
    switch (classFormat) {
    case sixtyFourBitIndexableFormat():
      if (((instBytes & 7) != 0)) {
        return PrimErrBadReceiver;
      }
      newFormat = classFormat;
      break;
    case firstLongFormat():
      if (((instBytes & 3) != 0)) {
        return PrimErrBadReceiver;
      }
      newFormat = classFormat + ((2 - (((usqInt)(instBytes)) >> 2)) & 1);
      break;
    case firstShortFormat():
      if (((instBytes & 1) != 0)) {
        return PrimErrBadReceiver;
      }
      newFormat = classFormat + ((4 - (((usqInt)(instBytes)) >> 1)) & 3);
      break;
    case firstByteFormat():
      newFormat = classFormat + ((8 - instBytes) & 7);
      break;
    case firstCompiledMethodFormat():
      if (classFormat != (firstCompiledMethodFormat())) {
        return PrimErrInappropriate;
      }
      newFormat = instFormat;
      break;
    default:
      return PrimErrInappropriate;
    }
  }

  /* Fail if the class's format is somehow invalid. */

  /* begin set:classIndexTo:formatTo: */
  assert(((classIndex >= 0) && (classIndex <= (classIndexMask()))));
  assert(((newFormat >= 0) && (newFormat <= (formatMask()))));
  longAtput((void *)(rcvr),
            ((longAt((void *)(rcvr))) &
             (~(usqIntptr_t)(((((usqInt)((formatMask())) << (formatShift())))) +
                             (classIndexMask())))) +
                (classIndex + ((((usqInt)(newFormat) << (formatShift()))))));
  assert((numBytesOf(rcvr)) ==
         ((classFormat <= (lastPointerFormat()) ? instSlots * BytesPerOop
                                                : instBytes)));

  /* ok */
  return 0;
}