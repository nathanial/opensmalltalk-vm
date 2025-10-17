/* Extracted from interp.c:22757 (function primitiveSizeInBytesOfInstance). */

/*	Answer the byte size of an instance of the receiver. If num args > 0
        then the last argument is a variable size and the size answered is the
        size of an instance of the receiver with that many indexable elements.
 */

/* InterpreterPrimitives>>#primitiveSizeInBytesOfInstance */

static void primitiveSizeInBytesOfInstance(void) {
  usqInt byteSize;
  sqInt classFormat;
  int err;
  sqInt instSpec;
  usqInt numSlots;
  sqInt numSlotsSqInt;
  sqInt oop;
  char *sp;

  if (argumentCount > 1) {
    /* primitiveFailFor: */
    primFailCode = PrimErrBadNumArgs;
    return;
  }

  /* Support VMMirror>>byteSizeOfInstanceOf:WithIndexableVariables: */
  err = -1;
  if (argumentCount >= 1) {
    if (!(((((longAt(stackPointer))) & 7) == 1))) {
      /* primitiveFailFor: */
      primFailCode = PrimErrBadArgument;
      return;
    }

    /* begin byteSizeOfInstanceOf:withIndexableSlots:errInto: */
    classFormat =
        ((fetchPointerofObject(InstanceSpecificationIndex,
                               longAt(stackPointer + (1 * BytesPerWord)))) >>
         3);
    instSpec =
        (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
    switch (instSpec) {
    case arrayFormat():
      numSlots = ((longAt(stackPointer)) >> 3);
      break;
    case indexablePointersFormat():
    case weakArrayFormat():
      numSlots = (classFormat & ((1U << (fixedFieldsFieldWidth())) - 1)) +
                 (((longAt(stackPointer)) >> 3));
      break;
    case sixtyFourBitIndexableFormat():
      numSlots = ((longAt(stackPointer)) >> 3);
      break;
    case firstLongFormat():
      numSlots = ((((longAt(stackPointer)) >> 3)) + 1) / 2;
      break;
    case firstShortFormat():
      numSlots = ((((longAt(stackPointer)) >> 3)) + 3) / 4;
      break;
    case firstByteFormat():
    case firstCompiledMethodFormat():
      numSlots =
          ((((longAt(stackPointer)) >> 3)) + (BytesPerOop - 1)) / BytesPerOop;
      break;
    default:
      byteSize = (err = PrimErrBadReceiver);
      goto l1;
    }
    if (numSlots >=
        (((((usqInt)1)) << ((BytesPerOop * 8) - 3 /* logBytesPerOop */)))) {
      byteSize =
          (err = ((((longAt(stackPointer)) >> 3)) < 0 ? PrimErrBadArgument
                                                      : PrimErrLimitExceeded));
      goto l1;
    }
    byteSize = /* objectBytesForSlots: */
        (numSlots ? ((numSlots << (shiftForWord()))) +
                        ((numSlots >= (numSlotsMask())
                              ? BaseHeaderSize + BaseHeaderSize
                              : BaseHeaderSize))
                  : 8 /* allocationUnit */ + BaseHeaderSize);
    /* end byteSizeOfInstanceOf:withIndexableSlots:errInto: */
  l1:
    if (err >= 0) {
      /* primitiveFailFor: */
      primFailCode = err;
      return;
    }
    oop = positive64BitIntegerFor(byteSize);

    /* begin pop:thenPush: */
    longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),
              oop);
    stackPointer = sp;
    return;
  }

  /* begin byteSizeOfInstanceOf:errInto: */
  classFormat = ((fetchPointerofObject(InstanceSpecificationIndex,
                                       longAt(stackPointer))) >>
                 3);
  instSpec =
      (((usqInt)(classFormat)) >> (fixedFieldsFieldWidth())) & (formatMask());
  if (!(/* isFixedSizePointerFormat: */
        (instSpec <= (nonIndexablePointerFormat())) ||
        (instSpec == (ephemeronFormat())))) {
    byteSize = (err = PrimErrBadReceiver);
    goto l2;
  }
  numSlotsSqInt = classFormat & ((1U << (fixedFieldsFieldWidth())) - 1);
  byteSize = /* objectBytesForSlots: */
      (numSlotsSqInt ? ((((usqInt)(numSlotsSqInt) << (shiftForWord())))) +
                           ((numSlotsSqInt >= (numSlotsMask())
                                 ? BaseHeaderSize + BaseHeaderSize
                                 : BaseHeaderSize))
                     : 8 /* allocationUnit */ + BaseHeaderSize);
  /* end byteSizeOfInstanceOf:errInto: */
l2:
  if (err >= 0) {
    /* primitiveFailFor: */
    primFailCode = err;
    return;
  }

  /* begin pop:thenPushInteger: */
  longAtput((sp = stackPointer), ((byteSize << 3) | 1));
  stackPointer = sp;
}